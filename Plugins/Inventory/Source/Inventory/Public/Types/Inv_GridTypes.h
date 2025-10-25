#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
    Equippable,
    Consumable,
    Craftable,
    None,
};

USTRUCT()
struct FInv_SlotAvailability
{
    GENERATED_BODY()

    FInv_SlotAvailability() {}
    FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

    int32 Index{INDEX_NONE}; // What index are we pitting an item in how much of that item are we filling in, and is there already
    int32 AmountToFill{0};
    bool bItemAtIndex{false};
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
    GENERATED_BODY()

    FInv_SlotAvailabilityResult() {}

    TWeakObjectPtr<UInv_InventoryItem> Item; // Is there an item of this type already in the inventory?
    int32 TotalRoomToFill{0}; // How much room can we accommodate for?
    int32 Remainder{0}; // how much we're going to have left over that can't fit in the inventory?
    bool bStackable{false}; // is this a stackable item?
    TArray<FInv_SlotAvailability> SlotAvailabilities; // the slot availabilities is an array of more infomation that we have to know
};