// Fill out your copyright notice in the Description page of Project Settings.


#include "XYZBaseCharacterAnimInstance.h"
#include "../XYZBaseCharacter.h"
#include "../MovementComponents/XYZBaseCharacterMovementComp.h"
#include "../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "Actors/Equipment/Weapons/RangeWeaponItem.h"

void UXYZBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AXYZBaseCharacter>(), TEXT("UXYZBaseCharacterAnimInstance::NativeBeginPlay() can be used inly with AXYZBaseCharacter"));
	CachedBaseCharacter = StaticCast<AXYZBaseCharacter*>(TryGetPawnOwner());

}

void UXYZBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	bIsAiming = CachedBaseCharacter->IsAiming();

	UXYZBaseCharacterMovementComp* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();

	bIsStrafing = !CharacterMovement->bOrientRotationToMovement;
	Direction = CalculateDirection(CharacterMovement->Velocity, CachedBaseCharacter->GetActorRotation());


	CurrentStamina = CachedBaseCharacter->IsOutOfStamina();
	Speed = CharacterMovement->Velocity.Size();
	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsOutOfStamina = CachedBaseCharacter->IsOutOfStamina();
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsSwimming = CharacterMovement->IsSwimming();
	bIsOnLadder = CharacterMovement->IsOnLadder();
	if (bIsOnLadder)
	{
		LadderSpeedRatio = CharacterMovement->GetLadderSpeedRatio();
	}

	AimRotation = CachedBaseCharacter->GetBaseAimRotation();

	const UCharacterEquipmentComponent* CharacterEquipment = CachedBaseCharacter->GetCharacterEquipmentComponent();
	CurrentEquippedItemType = CharacterEquipment->GetCurrentEquippedItemType();

	ARangeWeaponItem* CurrentRangeWeapon = CharacterEquipment->GetCurrentRangeWeapon();
	if (IsValid(CurrentRangeWeapon))
	{
		ForeGripSocketTransform = CurrentRangeWeapon->GetForeGripTransform();
	}
}

