#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "GameFramework/Actor.h"

#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Types/Inv_GridTypes.h"

TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
    TArray<UInv_InventoryItem*> Results;
    Results.Reserve(Entries.Num());
    for (const auto& Entry : Entries)
    {
        if (!IsValid(Entry.Item)) continue;
        Results.Add(Entry.Item);
    }
    return Results;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
    if (!IsValid(InventoryComponent)) return;

    for (int32 Index : RemovedIndices)
    {
        InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
    }
}

void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
    if (!IsValid(InventoryComponent)) return;

    for (int32 Index : AddedIndices)
    {
        InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);
    }
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_ItemComponent* ItemComponent)
{
    // TODO : Implement once ItemComponent is more complete.
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    check(OwningActor->HasAuthority());

    UInv_InventoryComponent* InventoryComp = Cast<UInv_InventoryComponent>(OwnerComponent);
    if (!IsValid(InventoryComp)) return nullptr;

    FInv_InventoryEntry NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.Item                = ItemComponent->GetItemManifest().NewManifest(OwningActor);

    InventoryComp->AddRepSubObj(NewEntry.Item);
    MarkItemDirty(NewEntry);

    return NewEntry.Item;
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryItem* Item)
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    check(OwningActor->HasAuthority());

    FInv_InventoryEntry NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.Item                = Item;

    MarkItemDirty(NewEntry);

    return Item;
}

void FInv_InventoryFastArray::RemoveEntry(UInv_InventoryItem* Item)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FInv_InventoryEntry& Entry = *EntryIt;
        if (Entry.Item == Item)
        {
            EntryIt.RemoveCurrent();
            MarkItemDirty(Entry);
            break;
        }
    }
}
