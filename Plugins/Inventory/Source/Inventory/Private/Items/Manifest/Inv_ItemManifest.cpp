#include "Items/Manifest/Inv_ItemManifest.h"

#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemManifest::NewManifest(UObject* NewOuther)
{
    UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuther, UInv_InventoryItem::StaticClass());
    Item->SetItemManifest(*this);

    return Item;
}
