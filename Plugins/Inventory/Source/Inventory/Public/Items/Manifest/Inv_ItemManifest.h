#pragma once

#include "CoreMinimal.h"

#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Items/Inv_ItemTags.h"

#include "Inv_ItemManifest.generated.h"

class UInv_InventoryItem;

/**
 * The Item Manifest contains all of the necessary data for creating a new Inventory Item
 */

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
    GENERATED_BODY()

    FInv_ItemManifest() {}

    // Create New Inventory Item
    UInv_InventoryItem* NewManifest(UObject* NewOuther);

    EInv_ItemCategory GetItemCategory() const { return Category; }
    FGameplayTag GetItemType() const { return ItemType; }


private:
    UPROPERTY(EditAnywhere, Category = "Inventory")
    EInv_ItemCategory Category{EInv_ItemCategory::None};

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FGameplayTag ItemType;
};

// inline UInv_InventoryItem* FInv_ItemManifest::NewManifest(UObject* NewOuter)
// {
// }