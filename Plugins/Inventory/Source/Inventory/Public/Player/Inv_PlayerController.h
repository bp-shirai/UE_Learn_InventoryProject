// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;

/**
 *
 */
UCLASS(Abstract)
class INVENTORY_API AInv_PlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    AInv_PlayerController();

protected:
    virtual void BeginPlay() override;
    
    /** Input mapping context setup */
    virtual void SetupInputComponent() override;

    virtual void Tick(float DeltaTime) override;

private:
    void Input_PrimaryInteract();
    void CreateHUDWidget();
    void TraceForItem();
    
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TObjectPtr<UInputAction> PrimaryInteractAction;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

    UPROPERTY(Transient)
    TObjectPtr<UInv_HUDWidget> HUDWidget;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    float TraceLength = 500.f;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TEnumAsByte<ECollisionChannel> ItemTraceChannel = ECC_Visibility;

    TWeakObjectPtr<AActor> ThisActor;
    TWeakObjectPtr<AActor> LastActor;

  
};
