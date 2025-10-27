// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#include "Types/Inv_GridTypes.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Items/Components/Inv_ItemComponent.h"

void UInv_SpatialInventory::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
    Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
    Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

    ShowEquippables();
}

void UInv_SpatialInventory::ShowEquippables()
{
    SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventory::ShowConsumables()
{
    SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventory::ShowCraftables()
{
    SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button)
{
    DisableButton(Button);

    InventorySwitcher->SetActiveWidget(Grid);
}

void UInv_SpatialInventory::DisableButton(UButton* Button)
{
    Button_Equippables->SetIsEnabled(true);
    Button_Consumables->SetIsEnabled(true);
    Button_Craftables->SetIsEnabled(true);
    Button->SetIsEnabled(false);
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
    switch (UInv_WidgetUtils::GetItemCategoryFromItemComponent(ItemComponent))
    {
        case EInv_ItemCategory::Equippable: return Grid_Equippables->HasRoomForItem(ItemComponent);
        case EInv_ItemCategory::Consumable: return Grid_Consumables->HasRoomForItem(ItemComponent);
        case EInv_ItemCategory::Craftable: return Grid_Craftables->HasRoomForItem(ItemComponent);
        default: UE_LOG(LogTemp, Error, TEXT("ItemComponent doesn't have a valid Item Category.")); return FInv_SlotAvailabilityResult();
    }
}
