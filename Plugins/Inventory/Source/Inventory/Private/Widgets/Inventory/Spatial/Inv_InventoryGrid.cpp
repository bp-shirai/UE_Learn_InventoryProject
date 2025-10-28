// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Engine/Texture2D.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Math/MathFwd.h"
#include "Styling/SlateBrush.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
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
            GridSlot->SetIndex(Index);

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

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
    return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item)
{
    return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
    FInv_SlotAvailabilityResult Result;

    // Determine if the item is stackable.
    const FInv_StackableFragment* StackableFragment = Manifest.GetFragment<FInv_StackableFragment>();
    Result.bStackable                               = StackableFragment != nullptr;

    // Determine how many stack to add.
    const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
    int32 AmountToFill       = StackableFragment ? StackableFragment->GetStackCount() : 1;

    TSet<int32> CheckedIndices;
    // For each Grid Slot:
    for (const auto& GridSlot : GridSlots)
    {
        // If we don't have anymore to fill, break out of the loop early.
        if (AmountToFill == 0) break;

        // Is this index claimed yet?
        if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue;

        // Can the item fit here? (i.e. is it out of grid bounds?)
        TSet<int32> TentativelyClaimed;

        if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativelyClaimed))
        {
            continue;
        }

        CheckedIndices.Append(TentativelyClaimed);

        // Is there room at this index? (i.e. are there other items in the way?)
        // Check any other important conditions - FroEach2D over a 2D range
        // Index claimed?
        // Has valid item?
        // Is this item the same type as the item we're trying to add?
        // If so, is this a stackable item?
        // If stackable, is this slot at the max stack size already?
        // How much to fill?
        // Update the amount left to fill
    }
    // How much is the Remainder?

    return Result;
}

void UInv_InventoryGrid::AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem)
{
    for (const auto& Availability : Result.SlotAvailabilities)
    {
        AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
        UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
    }
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
    const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
    return GridFragment->GetGridSize() * IconTileWidth;
}

void UInv_InventoryGrid::SetSlottedItemImage(UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment)
{
    FSlateBrush Brush;
    Brush.SetResourceObject(ImageFragment->GetIcon());
    Brush.DrawAs    = ESlateBrushDrawType::Image;
    Brush.ImageSize = GetDrawSize(GridFragment);
    SlottedItem->SetImageBrush(Brush);
}

void UInv_InventoryGrid::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount)
{
    // Get Grid Fragment so we know how many grid space the item takes.
    const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::Grid);

    // Get Image Fragment so we have the icon to display.
    const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::Image);

    if (!GridFragment || !ImageFragment) return;

    // Create a widget to add to the gird
    UInv_SlottedItem* SlottedItem = CreateSlottedItem(Item, Index, bStackable, StackAmount, GridFragment, ImageFragment);

    // Add the slotted item to the canvas panel.
    AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

    // Store the new widget in a container.
    SlottedItems.Add(Index, SlottedItem);
}

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment)
{
    checkf(SlottedItemClass, TEXT("SlottedItemClass should be set"));
    UInv_SlottedItem* SlottedItem = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
    SlottedItem->SetInventoryItem(Item);
    SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
    SlottedItem->SetGridIndex(Index);

    SlottedItem->SetStackable(bStackable);
    const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
    SlottedItem->UpdateStackCount(StackUpdateAmount);

    return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem)
{
    CanvasPanel->AddChild(SlottedItem);
    UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
    CanvasSlot->SetSize(GetDrawSize(GridFragment));

    const FVector2D DrawPos            = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
    const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
    CanvasSlot->SetPosition(DrawPosWithPadding);
}

void UInv_InventoryGrid::UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
    check(GridSlots.IsValidIndex(Index));

    if (bStackableItem)
    {
        GridSlots[Index]->SetStackCount(StackAmount);
    }

    const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::Grid);
    const FIntPoint Dimensions            = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

    UInv_InventoryStatics::ForEach2D(
        GridSlots, Index, Dimensions, Columns,
        [&](UInv_GridSlot* GridSlot)
        {
            GridSlot->SetInventoryItem(NewItem);
            GridSlot->SetUpperLeftIndex(Index);
            GridSlot->SetOccupiedTexture();
            GridSlot->SetAvailable(false);
        });
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
    return CheckedIndices.Contains(Index);
}

bool UInv_InventoryGrid::HasRoomAtIndex(const UInv_GridSlot* GridSlot, const FIntPoint& Dimensions, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed)
{
    // Is there room at this index? (i.e. are there other items in the way?)
    bool bHasRoomAtIndex = true;

    UInv_InventoryStatics::ForEach2D(
        GridSlots, GridSlot->GetIndex(), Dimensions, Columns,
        [&](UInv_GridSlot* SubGridSlot)
        {
            if (CheckSlotConstraints(SubGridSlot))
            {
                OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
            }
            else
            {
                bHasRoomAtIndex = false;
            }
        });

    return bHasRoomAtIndex;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
    const FInv_GridFragment* GridFragment = Manifest.GetFragment<FInv_GridFragment>();
    return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UInv_InventoryGrid::CheckSlotConstraints(const UInv_GridSlot* SubGridSlot) const
{
    // Check any other important conditions - FroEach2D over a 2D range
    // Index claimed?
    // Has valid item?
    // Is this item the same type as the item we're trying to add?
    // If so, is this a stackable item?
    // If stackable, is this slot at the max stack size already?
    return false;
}
