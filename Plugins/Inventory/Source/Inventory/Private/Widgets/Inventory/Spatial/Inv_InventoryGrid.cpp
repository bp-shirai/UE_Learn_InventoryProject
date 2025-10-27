// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Items/Fragments/Inv_ItemFragment.h"
#include "Templates/UnrealTemplate.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"

#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ConstructGrid();

    InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
    InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

void UInv_InventoryGrid::ConstructGrid()
{
    checkf(IsValid(GridSlotClass), TEXT("GridSlotClass should be set UInv_InventoryGrid class"));

    GridSlots.Reserve(Rows * Columns);

    for (int32 j = 0; j < Rows; j++)
    {
        for (int32 i = 0; i < Columns; i++)
        {
            UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
            CanvasPanel->AddChild(GridSlot);

            const FIntPoint TilePosition(i, j);
            const int32 Index = UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns);
            GridSlot->SetTileIndex(Index);

            UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
            CanvasSlot->SetSize(FVector2D(TileSize));
            CanvasSlot->SetPosition(TilePosition * TileSize);

            // UE_LOG(LogTemp, Warning, TEXT("%s"), *TilePosition.ToString());
            GridSlots.Add(GridSlot);
        }
    }
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
    if (!MatchesCategory(Item)) return;

    FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);
 
    // Create a widget to show the item icon and add it to the correct spot on the grid.
    AddItemToIndices(Result, Item);
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
    return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent) const
{
    return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item) const
{
    return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest) const
{
    FInv_SlotAvailabilityResult Result;
    Result.TotalRoomToFill = 1;

    FInv_SlotAvailability SlotAvailability;
    SlotAvailability.AmountToFill = 1;
    SlotAvailability.Index = 0;
    Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));

    return Result;
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem)
{
    // Get Grid Fragment so we know how many grid space the item takes.
    const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::Grid);

    // Get Image Fragment so we have the icon to display.

    // Create a widget to add to the gird

    // Store the new widget in a container.
}
