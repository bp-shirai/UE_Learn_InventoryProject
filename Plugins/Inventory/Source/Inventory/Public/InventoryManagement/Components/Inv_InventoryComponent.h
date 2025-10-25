// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventoryManagement/FastArray/Inv_FastArray.h"

#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryBase;
class UInv_InventoryItem;
class UInv_ItemComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInv_InventoryComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
    void TryAddItem(UInv_ItemComponent* ItemComponent);

    UFUNCTION(Server, Reliable)
    void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount);

    UFUNCTION(Server, Reliable)
    void Server_AddStacksToItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

    void AddRepSubObj(UObject* SubObj);

    void ToggleInventoryMenu();

    FInventoryItemChange OnItemAdded;
    FInventoryItemChange OnItemRemoved;
    FNoRoomInInventory NoRoomInInventory;

protected:
    virtual void BeginPlay() override;

private:
    void ConstructInventory();

    TWeakObjectPtr<APlayerController> OwningPlayerController;


    UPROPERTY(Replicated)
    FInv_InventoryFastArray InventoryList;

    UPROPERTY(Transient)
    TObjectPtr<UInv_InventoryBase> InventoryMenu;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<UInv_InventoryBase> InventoryMenuClass;

    bool bInventoryMenuOpen;
    void OpenInventoryMenu();
    void CloseInventoryMenu();
};
