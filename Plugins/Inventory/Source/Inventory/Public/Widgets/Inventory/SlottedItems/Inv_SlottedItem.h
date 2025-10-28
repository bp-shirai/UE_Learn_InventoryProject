// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItem.generated.h"

class UImage;
class UInv_InventoryItem;
class UTextBlock;

/**
 *
 */
UCLASS(Abstract)
class INVENTORY_API UInv_SlottedItem : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetImageBrush(const FSlateBrush& Brush);

    void SetInventoryItem(UInv_InventoryItem* Item) { InventoryItem_Ref = Item; }
    UInv_InventoryItem* GetInventoryItem() const { return InventoryItem_Ref.Get(); }
    void SetGridIndex(int32 Index) { GridIndex = Index; }
    int32 GetGridIndex() const { return GridIndex; }
    void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
    FIntPoint GetGridDimensions() const { return GridDimensions; }
    void SetStackable(bool Stackable) { bStackable = Stackable; }
    bool IsStackable() const { return bStackable; }

    void UpdateStackCount(int32 StackCount);

private:
    UPROPERTY()
    TWeakObjectPtr<UInv_InventoryItem> InventoryItem_Ref;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Image_Icon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_StackCount;

    int32 GridIndex;

    FIntPoint GridDimensions;

    bool bStackable{false};
};
