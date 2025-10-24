// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Inv_PlayerController.h"

#include "Components/ActorComponent.h"
#include "Engine/HitResult.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Interaction/Inv_Highlightable.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Interaction/Inv_HighlightableStaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"
#include "UnrealClient.h"

#include "Widgets/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    // PrimaryActorTick.TickInterval = 0.0f; // 毎フレーム（デフォルト）
}

void AInv_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();

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

        EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ThisClass::PrimaryInteract);
        EnhancedInputComponent->BindAction(ToggleInventoryMenuAction, ETriggerEvent::Started, this, &ThisClass::ToggleInventoryMenu);
    }
}

void AInv_PlayerController::PrimaryInteract()
{
    // UE_LOG(LogTemp, Log, TEXT("AInv_PlayerController::PrimaryInteract"));

    if (!ThisActor.IsValid()) return;

    UInv_ItemComponent* ItemComp           = ThisActor->FindComponentByClass<UInv_ItemComponent>();
    UInv_InventoryComponent* InventoryComp = InventoryComponent.Get();
    if (ItemComp && InventoryComp)
    {
        InventoryComp->TryAddItem(ItemComp);
    }
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
    else
    {
        PrimaryActorTick.bCanEverTick = false;
    }
}

void AInv_PlayerController::TraceForItem()
{
    if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;

    FVector2D ViewportSize;
    GEngine->GameViewport->GetViewportSize(ViewportSize);
    const FVector2D ViewportCenter = ViewportSize / 2.f;

    FVector TraceStart;
    FVector Forward;
    if (UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward))
    {
        const FVector TraceEnd = TraceStart + Forward * TraceLength;

        FHitResult HitResult;
        GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

        LastActor = ThisActor;
        ThisActor = HitResult.GetActor();

        // if (!ThisActor.IsValid())
        // {
        //     if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
        // }

        if (ThisActor == LastActor) return;

        if (ThisActor.IsValid())
        {
            // UE_LOG(LogTemp, Warning, TEXT("Started tracing a new actor."));

            if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
            {
                IInv_Highlightable::Execute_Highlight(Highlightable);
            }

            UInv_ItemComponent* ItemComp = ThisActor->FindComponentByClass<UInv_ItemComponent>();
            if (ItemComp)
            {
                if (HUDWidget) HUDWidget->ShowPickupMessage(ItemComp->GetPickupMessage());
            }
        }

        if (LastActor.IsValid())
        {
            // UE_LOG(LogTemp, Warning, TEXT("Stopped tracing last actor."));
            if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
            {
                IInv_Highlightable::Execute_UnHighlight(Highlightable);
            }

            if (HUDWidget) HUDWidget->HidePickupMessage();
        }
    }
}

void AInv_PlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TraceForItem();
}

void AInv_PlayerController::ToggleInventoryMenu()
{
    UInv_InventoryComponent* InventoryComp = InventoryComponent.Get();
    if (InventoryComp)
    {
        InventoryComp->ToggleInventoryMenu();
    }
}
