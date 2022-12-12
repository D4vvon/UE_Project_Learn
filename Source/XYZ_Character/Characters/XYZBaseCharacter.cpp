// Fill out your copyright notice in the Description page of Project Settings.


#include "XYZBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CharacterComponents/LedgeDetectorComponent.h"
#include "../Actors/Interactive/Environment/Ladder.h"
#include "Components/CharacterComponents/CharacterAttributeComponent.h"
#include "../XYZTypes.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Actors/Interactive/Interface/Interactable.h"
#include "Engine/EngineTypes.h"
#include "Components/CharacterComponents/CharacterInventoryComponent.h"
#include "Inventory/Items/InventoryItem.h"


AXYZBaseCharacter::AXYZBaseCharacter(const FObjectInitializer& ObjectInintializer)
	: Super(ObjectInintializer.SetDefaultSubobjectClass<UXYZBaseCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{
	XYZBaseCharacterMovementComponent = StaticCast<UXYZBaseCharacterMovementComp*>(GetCharacterMovement());

	CurrentStamina = MaxStamina;
	bIsFalling = GetBaseCharacterMovementComponent()->IsFalling();

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));

	CharacterAttributesComponent = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("CharacterAttributes"));
	CharacterInventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>(TEXT("CharacterInventory"));
	CharacterEquipmentComponent = CreateDefaultSubobject<UCharacterEquipmentComponent>(TEXT("CharacterEquipment"));
}


void AXYZBaseCharacter::EndPlay(const EEndPlayReason::Type Reason)
{
	if (OnInteractableObjectFound.IsBound())
	{
		OnInteractableObjectFound.Unbind();
	}
	Super::EndPlay(Reason);
}

void AXYZBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AIController = Cast<AAIController>(NewController);
	if (IsValid(AIController))
	{
		FGenericTeamId TeamId((uint8)Team);
		AIController->SetGenericTeamId(TeamId);
	}
}

void AXYZBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAttributesComponent->OnDeathEvent.AddUObject(this, &AXYZBaseCharacter::OnDeath);
}

bool AXYZBaseCharacter::IsOutOfStamina()
{
	if (CurrentStamina <= 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void AXYZBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else if(!bIsFalling)
	{
		Crouch();
	}
}

void AXYZBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void AXYZBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void AXYZBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TryChangeSprintStart(DeltaTime);
	

	if (CurrentStamina < MaxStamina)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Stamina: %.2f"), CurrentStamina));
	}


	if (XYZBaseCharacterMovementComponent->IsSprinting())
	{
		CurrentStamina -= SprintStaminaConsuptionVelocity * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
		if (CurrentStamina == 0)
		{
			XYZBaseCharacterMovementComponent->StopSprint();
			OnSprintEnd();
			
		}
	}
	else
	{
		CurrentStamina += StaminaRestoreVelocity * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}

	TraceLineOfSight();
}


void AXYZBaseCharacter::Mantle()
{
	FLedgeDescription LedgeDescription;
	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		FMantlingMovementParameters MantlingParameters;
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotation = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TargetRotation = LedgeDescription.Rotation;

		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHeight);

		float MinRange;
		float MaxRange;
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);

		MantlingParameters.Duration = MaxRange - MinRange;

		MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;
		
		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParameters.InintialAnimationLocation = MantlingParameters.TargetLocation - MantlingSettings.AnimationCorrectionZ * FVector::UpVector + MantlingSettings.AnimationCorrectionXY * LedgeDescription.LedgeNormal;

		XYZBaseCharacterMovementComponent->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.0f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
	}
}

UCharacterEquipmentComponent* AXYZBaseCharacter::GetCharacterEquipmentComponent_Mutable()
{
	return CharacterEquipmentComponent;
}

const UCharacterEquipmentComponent* AXYZBaseCharacter::GetCharacterEquipmentComponent() const
{
	return CharacterEquipmentComponent;
}

const UCharacterAttributeComponent* AXYZBaseCharacter::GetCharacterAttributesComponent() const
{
	return CharacterAttributesComponent;
}

void AXYZBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvalibleInteractiveActors.AddUnique(InteractiveActor);
}

void AXYZBaseCharacter::UnregisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvalibleInteractiveActors.RemoveSingleSwap(InteractiveActor);
}

const class ALadder* AXYZBaseCharacter::GetAvalibleLadder() const
{
	const ALadder* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvalibleInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	return Result;
}

bool AXYZBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GetBaseCharacterMovementComponent()->IsMantling();
}

bool AXYZBaseCharacter::CanSprint()
{
	return true;
}

void AXYZBaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	float Duration = PlayAnimMontage(OnDeathAnimMontage);
	if (Duration == 0)
	{
		EnableRagdoll();
	}

}

void AXYZBaseCharacter::OnStartAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
	{
		OnAimingStateChanged.Broadcast(true);
	}
}

void AXYZBaseCharacter::OnStopAimingInternal()
{
	if (OnAimingStateChanged.IsBound())
	{
		OnAimingStateChanged.Broadcast(false);
	}
}

void AXYZBaseCharacter::TraceLineOfSight()
{
	if (!IsPlayerControlled())
	{
		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;

	APlayerController* PlayerController = GetController<APlayerController>();
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector ViewDirection = ViewRotation.Vector();
	FVector TraceEnd = ViewLocation + ViewDirection * LineOfSightDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECC_Visibility);

	if (LineOfSightObject.GetObject() != HitResult.GetActor())
	{
		LineOfSightObject = HitResult.GetActor();

		FName ActionName;
		if (LineOfSightObject.GetInterface())
		{	
			ActionName = LineOfSightObject->GetActionEventName();
		}
		else
		{
			ActionName = FName(NAME_None);
		}
		OnInteractableObjectFound.ExecuteIfBound(ActionName);
	}
}

void AXYZBaseCharacter::TryChangeSprintStart(float DeltaTime)
{
	IsOutOfStamina();
	if (bIsSprintRequested && !XYZBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		XYZBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && XYZBaseCharacterMovementComponent->IsSprinting())
	{
		XYZBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}

const FMantlingSettings& AXYZBaseCharacter::GetMantlingSettings(float LedgeHeight)
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}

void AXYZBaseCharacter::EnableRagdoll()
{
	GetMesh()->SetCollisionProfileName(CollisionProfileRagdoll);
	GetMesh()->SetSimulatePhysics(true);
}

void AXYZBaseCharacter::ClimbLadderUp(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void AXYZBaseCharacter::InteractiveWithLadder()
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		GetBaseCharacterMovementComponent()->DetachFromLadder();
	}
	else
	{
		const ALadder* AvalibleLadder = GetAvalibleLadder();
		if (IsValid(AvalibleLadder))
		{
			//if (AvalibleLadder->GetIsOnTop())
			//{
			//	PlayAnimMontage(AvalibleLadder->GetAttachFromTopAnimMontage());
			//}
			GetBaseCharacterMovementComponent()->AttachToLadder(AvalibleLadder);
		}
	}
}

void AXYZBaseCharacter::Falling()
{
	GetBaseCharacterMovementComponent()->bNotifyApex = true;
}

void AXYZBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float FallHeight = (CurrentFallApex - GetActorLocation()).Z * 0.01f;
	if (IsValid(FallDamageCurve))
	{
		float DamageAmount = FallDamageCurve->GetFloatValue(FallHeight);
		TakeDamage(DamageAmount, FDamageEvent(), GetController(), Hit.GetActor());
	}
}

void AXYZBaseCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentFallApex = GetActorLocation();
}

void AXYZBaseCharacter::StartFire()
{
	if (CharacterEquipmentComponent->IsEquipping())
	{
		return;
	}
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StartFire();
	}
}

void AXYZBaseCharacter::StopFire()
{
	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipmentComponent->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopFire();
	}
}

void AXYZBaseCharacter::StartAiming()
{
	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (!IsValid(CurrentRangeWeapon))
	{
		return;
	}
	bIsAiming = true;
	CurrentAimingMovementSpeed = CurrentRangeWeapon->GetAimMovementMaxSpeed();
	CurrentRangeWeapon->StartAim();
	OnStartAiming();
}

void AXYZBaseCharacter::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	ARangeWeaponItem* CurrentRangeWeapon = GetCharacterEquipmentComponent()->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		CurrentRangeWeapon->StopAim();

	}
	bIsAiming = false;
	CurrentAimingMovementSpeed = 0.0f;
	OnStopAiming();
}

void AXYZBaseCharacter::NextItem()
{
	CharacterEquipmentComponent->EquipNextItem();
}

void AXYZBaseCharacter::PreviousItem()
{
	CharacterEquipmentComponent->EquipPreviousItem();
}

void AXYZBaseCharacter::Reload()
{
	if (IsValid(CharacterEquipmentComponent->GetCurrentRangeWeapon()))
	{
		CharacterEquipmentComponent->ReloadCurrentWeapon();
	}
}

void AXYZBaseCharacter::EquipPrimaryItem()
{
	CharacterEquipmentComponent->EquipItemInSlot(EEquipmentSlots::PrimaryItemSlot);
}

void AXYZBaseCharacter::OnStartAiming_Implementation()
{
	OnStartAimingInternal();
}

void AXYZBaseCharacter::OnStopAiming_Implementation()
{
	OnStopAimingInternal();
}

float AXYZBaseCharacter::GetAimingMovementSpeed() const
{
	return CurrentAimingMovementSpeed;
}

bool AXYZBaseCharacter::IsAiming() const
{
	return bIsAiming;
}

FGenericTeamId AXYZBaseCharacter::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)Team);
}

void AXYZBaseCharacter::Interact()
{
	if (LineOfSightObject.GetInterface())
	{
		LineOfSightObject->Interact(this);
	}
}

//void AXYZBaseCharacter::AddEquipmentItem(const TSubclassOf<class AEquipableItem> EquipableItemClass)
//{
//	CharacterEquipmentComponent->AddEquipmentItemToSlot(EquipableItemClass, TODO);
//}

bool AXYZBaseCharacter::PickupItem(TWeakObjectPtr<UInventoryItem> ItemToPickup)
{
	bool Result = false;
	if (CharacterInventoryComponent->HasFreeSlot())
	{
		CharacterInventoryComponent->AddItem(ItemToPickup, 1);
		Result = true;
	}
	return Result;
}

void AXYZBaseCharacter::UseInventory(APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		return;
	}

	//APlayerController* PlayerController = GetController<APlayerController>();

	if (!CharacterInventoryComponent->IsViewVisible())
	{
		CharacterInventoryComponent->OpenViewInventory(PlayerController);
		CharacterEquipmentComponent->OpenViewEquipment(PlayerController);
		PlayerController->SetInputMode(FInputModeGameAndUI{});
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		CharacterInventoryComponent->CloseViewInventory();
		CharacterEquipmentComponent->CloseViewEquipment();
		PlayerController->SetInputMode(FInputModeGameOnly{});
		PlayerController->bShowMouseCursor = false;
	}
}

