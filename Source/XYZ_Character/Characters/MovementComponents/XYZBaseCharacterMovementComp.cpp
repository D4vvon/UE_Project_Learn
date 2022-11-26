// Fill out your copyright notice in the Description page of Project Settings.


#include "XYZBaseCharacterMovementComp.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../../Actors/Interactive/Environment/Ladder.h"
#include "../XYZBaseCharacter.h"



void UXYZBaseCharacterMovementComp::PhysicsRotation(float DeltaTime)
{
	if (IsOnLadder())
	{
		FRotator YawRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
		YawRotation.Pitch = 0.0f;
		YawRotation.Roll = 0.0f;
		YawRotation.Yaw += 180.0f;
		
		GetOwner()->SetActorRotation(FRotator(YawRotation));
		return;
	}
	Super::PhysicsRotation(DeltaTime);
}

float UXYZBaseCharacterMovementComp::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	else if (IsOnLadder())
	{
		Result = ClimbingOnLadderMaxSpeed;
	}
	else if (GetBaseCharacterOwner()->IsAiming())
	{
		Result = GetBaseCharacterOwner()->GetAimingMovementSpeed();
	}
	return Result;
}

void UXYZBaseCharacterMovementComp::StartSprint()
{
	bIsSprinting = true;

	bForceMaxAccel = 1;
}

void UXYZBaseCharacterMovementComp::StopSprint()
{
	bIsSprinting = false;

	bForceMaxAccel = 0;
}

void UXYZBaseCharacterMovementComp::SetIsOutOfStamina(bool bIsOutOfStamina_In)
{
	bIsOutOfStamina = bIsOutOfStamina_In;
	if (bIsOutOfStamina)
	{
		StopSprint();
	}
}

void UXYZBaseCharacterMovementComp::StartMantle(const FMantlingMovementParameters& MantlingParamenters)
{
	if (bIsMantling == false)
	{
		CurrentMantlingParameters = MantlingParamenters;
		bIsMantling = true;
		SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
	}
}

void UXYZBaseCharacterMovementComp::EndMantle()
{
	bIsMantling = false;
	SetMovementMode(MOVE_Walking);
}

bool UXYZBaseCharacterMovementComp::IsMantling() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Mantling;
}

void UXYZBaseCharacterMovementComp::AttachToLadder(const ALadder* Ladder)
{
	CurrentLadder = Ladder;

	FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrientationRotation.Yaw += 0.0f;


	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	float Projection = GetActorToCurrentLadderProjection(GetActorLocation());


	//FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector;
	//if (CurrentLadder->GetIsOnTop())
	//{
	//	NewCharacterLocation = CurrentLadder->GetAttachFromTopAnimMontageStartingLocation();
	//}


	//GetOwner()->SetActorLocation(NewCharacterLocation);
	//GetOwner()->SetActorRotation(FRotator(0.0f, TargetOrientationRotation.Yaw, 0.0f));

	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Ladder);
}

float UXYZBaseCharacterMovementComp::GetActorToCurrentLadderProjection(const FVector& Location) const
{

	checkf(IsValid(CurrentLadder), TEXT("UXYZBaseCharacterMovementComp::GetCharacterToCurrentLadderProjection() cant be worked when currentladder is null"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderToCharacterDistance = Location - CurrentLadder->GetActorLocation();
	return FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);
}

float UXYZBaseCharacterMovementComp::GetLadderSpeedRatio() const
{
	checkf(IsValid(CurrentLadder), TEXT("UXYZBaseCharacterMovementComp::GetLadderSpeedRatio() cant be worked when currentladder is null"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return FVector::DotProduct(LadderUpVector, Velocity) / ClimbingOnLadderMaxSpeed;

}

void UXYZBaseCharacterMovementComp::DetachFromLadder()
{
	SetMovementMode(MOVE_Falling);
}

bool UXYZBaseCharacterMovementComp::IsOnLadder() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::CMOVE_Ladder;
}

const ALadder* UXYZBaseCharacterMovementComp::GetCurrentLadder()
{
	return CurrentLadder;
}

void UXYZBaseCharacterMovementComp::PhysCustom(float DeltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
		case (uint8)ECustomMovementMode::CMOVE_Mantling:
			{
				PhysMantling(DeltaTime, Iterations);
				break;
			}
			case (uint8)ECustomMovementMode::CMOVE_Ladder:
			{
				PhysLadder(DeltaTime, Iterations);
				break;
			}
		default:
			break;
	}

	Super::PhysCustom(DeltaTime, Iterations);
}

void UXYZBaseCharacterMovementComp::PhysMantling(float DeltaTime, int32 Iterations)
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

	FVector MantleCurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);

	float PositionAlpha = MantleCurveValue.X;
	float XYCorrectionAlpha = MantleCurveValue.Y;
	float ZCorrectionAlpha = MantleCurveValue.Z;

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, CurrentMantlingParameters.InintialAnimationLocation, XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, CurrentMantlingParameters.InintialAnimationLocation.Z, ZCorrectionAlpha);

	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, CurrentMantlingParameters.TargetRotation, PositionAlpha);

	FVector Delta = NewLocation - GetActorLocation();
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
}

void UXYZBaseCharacterMovementComp::PhysLadder(float DeltaTime, int32 Iterations)
{
	CalcVelocity(DeltaTime, 1.0f, false, ClimbingOnLadderBreakingDecelaration);
	FVector Delta = Velocity * DeltaTime;

	FVector NewPos = GetActorLocation() + Delta;
	float NewPosProjection = GetActorToCurrentLadderProjection(NewPos);

	if (NewPosProjection < MinLadderBottomOffset)
	{
		DetachFromLadder();
		return;
	}
	else if (NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
	{
		GetBaseCharacterOwner()->Mantle();
		return;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetOwner()->GetActorRotation(), true, Hit);
}

void UXYZBaseCharacterMovementComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (MovementMode == MOVE_Swimming)
	{
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), true);

	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)ECustomMovementMode::CMOVE_Ladder)
	{
		CurrentLadder = nullptr;
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case (uint8)ECustomMovementMode::CMOVE_Mantling:
			{
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UXYZBaseCharacterMovementComp::EndMantle, CurrentMantlingParameters.Duration, false);
				break;
			}
			default:
				break;
		}
	}
}

AXYZBaseCharacter* UXYZBaseCharacterMovementComp::GetBaseCharacterOwner() const
{
	return StaticCast<AXYZBaseCharacter*>(CharacterOwner);
}

