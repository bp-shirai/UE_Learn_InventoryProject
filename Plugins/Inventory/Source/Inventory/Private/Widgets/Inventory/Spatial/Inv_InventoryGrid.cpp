// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "UObject/Object.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"


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

    UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem InventoryItem=%s"), *Item->GetName());

}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
    return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
