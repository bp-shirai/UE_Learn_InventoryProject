// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

#include "Items/Inv_InventoryItem.h"
#include "Types/Inv_GridTypes.h"

#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Items/Components/Inv_ItemComponent.h"

UInv_InventoryComponent::UInv_InventoryComponent()
    : InventoryList(this)
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
    bReplicateUsingRegisteredSubObjectList = true;

    bInventoryMenuOpen = false;
}

void UInv_InventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
    OwningPlayerController = Cast<APlayerController>(GetOwner());
    checkf(OwningPlayerController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner"));
    checkf(IsValid(InventoryMenuClass), TEXT("Inventory Menu Class should be set"));

    if (!OwningPlayerController->IsLocalController()) return;

    // Create inventory menu widget
    InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningPlayerController.Get(), InventoryMenuClass);
    InventoryMenu->AddToViewport();

    CloseInventoryMenu();
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
    if (bInventoryMenuOpen)
    {
        CloseInventoryMenu();
    }
    else
    {
        OpenInventoryMenu();
    }
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
    if (InventoryMenu)
    {
        InventoryMenu->SetVisibility(ESlateVisibility::Visible);
        bInventoryMenuOpen = true;

        if (APlayerController* PC = OwningPlayerController.Get())
        {
            FInputModeGameAndUI InputMode;
            // InputMode.SetWidgetToFocus(InventoryMenu->TakeWidget());
            // InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->SetShowMouseCursor(true);
        }
    }
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
    if (IsValid(InventoryMenu))
    {
        InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
        bInventoryMenuOpen = false;

        if (APlayerController* PC = OwningPlayerController.Get())
        {
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->SetShowMouseCursor(false);
        }
    }
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
    FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

    if (Result.TotalRoomToFill == 0)
    {
        NoRoomInInventory.Broadcast();
        return;
    }

    // TODO : Actually add the item to the inventory
    if (Result.Item.IsValid() && Result.bStackable)
    {
        // Add stacks to an item that already exists in the inventory. We only want to update the stack count.
        // not create a new item of this type.
        Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
    }
    else if (Result.TotalRoomToFill > 0)
    {
        // This item type doesn't exist in the inventory. Create a new one and update all pertinent slots.
        Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
    }
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount)
{
    UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);

    if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
    {
        OnItemAdded.Broadcast(NewItem);
    }

    // TODO: Tell the Item Component to destroy its owning actor.
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder)
{
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
    if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
    {
        AddReplicatedSubObject(SubObj);
    }
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, InventoryList);
}
