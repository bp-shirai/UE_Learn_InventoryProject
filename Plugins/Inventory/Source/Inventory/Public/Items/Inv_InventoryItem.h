// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Items/Manifest/Inv_ItemManifest.h"

#include "Inv_InventoryItem.generated.h"

/**
 *
 */
UCLASS()
class INVENTORY_API UInv_InventoryItem : public UObject
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual bool IsSupportedForNetworking() const override { return true; };

    void SetItemManifest(const FInv_ItemManifest& Manifest);
    const FInv_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FInv_ItemManifest>(); }
    FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FInv_ItemManifest>(); }

    int32 GetTotalStackCount() const { return TotalStackCount; }
    void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }

private:
    UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.Inv_ItemManifest"), Replicated, Category = "Inventory")
    FInstancedStruct ItemManifest;

    UPROPERTY(Replicated)
    int32 TotalStackCount{0};
};