// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/PlayerController.h"

#include "UObject/Object.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Items/Components/Inv_ItemComponent.h"

UInv_InventoryComponent::UInv_InventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
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
    NoRoomInInventory.Broadcast();
}
