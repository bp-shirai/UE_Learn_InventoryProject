// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "GameFramework/PlayerController.h"

#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"


UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}



void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}



void UInv_InventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayerController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner"));

	if (!OwningPlayerController->IsLocalController() || !InventoryMenuClass) return;

	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningPlayerController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	//InventryMenu->SetVisibility(ESlateVisibility::Hidden);

}
