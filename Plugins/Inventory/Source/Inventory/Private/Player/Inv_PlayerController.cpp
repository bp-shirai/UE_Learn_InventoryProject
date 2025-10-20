// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

#include "Widgets/HUD/Inv_HUDWidget.h"

void AInv_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    CreateHUDWidget();
}

void AInv_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // only add IMCs for local player controllers
    if (IsLocalPlayerController())
    {
        // Add Input Mapping Contexts
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
            {
                Subsystem->AddMappingContext(CurrentContext, 0);
            }
        }

        UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

        EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ThisClass::Input_PrimaryInteract);
    }
}

void AInv_PlayerController::Input_PrimaryInteract()
{
    UE_LOG(LogTemp, Log, TEXT("AInv_PlayerController::Input_PrimaryInteract"));
}

void AInv_PlayerController::CreateHUDWidget()
{
    if (IsLocalPlayerController())
    {
        HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
        if (IsValid(HUDWidget))
        {
            HUDWidget->AddToViewport(0);
        }
    }
}
