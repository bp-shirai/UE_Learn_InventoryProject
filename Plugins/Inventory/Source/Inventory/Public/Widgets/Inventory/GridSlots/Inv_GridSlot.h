// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;
class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
    Unoccupied,
    Occupied,
    Selected,
    GrayedOut,
};

/**
 *
 */
UCLASS(Abstract)
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetIndex(int32 Index) { TileIndex = Index; }
    int32 GetIndex() const { return TileIndex; }
    EInv_GridSlotState GetGridSlotState() const { return GridSlotState; }

    void SetInventoryItem(UInv_InventoryItem* Item);
    UInv_InventoryItem* GetInventoryItem() const { return InventoryItem_Ref.Get(); }
    void SetStackCount(int32 Count) { StackCount = Count; }
    int32 GetStackCount() const { return StackCount; }
    void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }
    int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
    void SetAvailable(bool Available) { bAvailable = Available; }
    bool IsAvailable() const { return bAvailable; }

    void SetUnoccupiedTexture();
    void SetOccupiedTexture();
    void SetSelectedTexture();
    void SetGrayedOutTexture();

private:
    int32 TileIndex{0};
    int32 StackCount{-1};
    int32 UpperLeftIndex{INDEX_NONE};
    bool bAvailable{false};

    UPROPERTY()
    TWeakObjectPtr<UInv_InventoryItem> InventoryItem_Ref;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Image_GridSlot;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FSlateBrush Brush_Unoccupied;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FSlateBrush Brush_Occupied;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FSlateBrush Brush_Selected;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FSlateBrush Brush_GrayedOut;

    EInv_GridSlotState GridSlotState;
};
