// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Inv_InventoryItem.h"

#include "Items/Manifest/Inv_ItemManifest.h"
#include "Net/UnrealNetwork.h"

void UInv_InventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, ItemManifest);
    DOREPLIFETIME(ThisClass, TotalStackCount)
}

void UInv_InventoryItem::SetItemManifest(const FInv_ItemManifest& Manifest)
{
    ItemManifest = FInstancedStruct::Make<FInv_ItemManifest>(Manifest);
}
