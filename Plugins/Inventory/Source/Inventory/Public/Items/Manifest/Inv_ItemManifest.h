#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"

#include "Types/Inv_GridTypes.h"
// #include "Items/Inv_ItemTags.h"
// #include "Items/Fragments/Inv_ItemFragment.h"

#include "Inv_ItemManifest.generated.h"

class UInv_InventoryItem;
struct FInv_ItemFragment;

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

    template <typename T>
        requires std::derived_from<T, FInv_ItemFragment>
    const T* GetFragmentByTag(const FGameplayTag& FragmentTag) const;

    template <typename T>
        requires std::derived_from<T, FInv_ItemFragment>
    const T* GetFragment() const;

private:
    UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct), Category = "Inventory")
    TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    EInv_ItemCategory Category{EInv_ItemCategory::None};

    UPROPERTY(EditAnywhere, meta = (Categories = "GameItems"), Category = "Inventory")
    FGameplayTag ItemType;
};

template <typename T>
    requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentByTag(const FGameplayTag& FragmentTag) const
{
    for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
    {
        if (const T* FragmentPtr = Fragment.GetPtr<T>())
        {
            if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
            return FragmentPtr;
        }
    }

    return nullptr;
}

template <typename T>
    requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragment() const
{
    for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
    {
        if (const T* FragmentPtr = Fragment.GetPtr<T>())
        {  
            return FragmentPtr;
        }
    }

    return nullptr;
}