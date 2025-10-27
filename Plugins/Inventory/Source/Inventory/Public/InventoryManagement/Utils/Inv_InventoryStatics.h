// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryStatics.generated.h"


class UInv_InventoryComponent;
class UInv_ItemComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory|Utils")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Utils")
    static EInv_ItemCategory GetItemCategoryFromItemComponent(const UInv_ItemComponent* ItemComponent);
};
