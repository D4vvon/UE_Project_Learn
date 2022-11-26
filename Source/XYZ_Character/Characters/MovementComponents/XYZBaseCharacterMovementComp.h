// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/CharacterComponents/LedgeDetectorComponent.h"
#include "Engine/EngineTypes.h"
#include "Curves/CurveVector.h"
#include "XYZBaseCharacterMovementComp.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector InintialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.0f;
	float StartTime = 0.0f;

	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_Max UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class XYZ_CHARACTER_API UXYZBaseCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool IsSprinting() { return bIsSprinting; }
	FORCEINLINE bool IsOutOfStamina() { return bIsOutOfStamina; }


	virtual void PhysicsRotation(float DeltaTime) override;

	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	void SetIsOutOfStamina(bool bIsOutOfStamina_In);

	void StartMantle(const FMantlingMovementParameters& MantlingParamenters);
	void EndMantle();
	bool IsMantling() const;

	void AttachToLadder(const class ALadder* Ladder);

	float GetActorToCurrentLadderProjection(const FVector& Location) const;

	float GetLadderSpeedRatio() const;

	void DetachFromLadder();
	bool IsOnLadder() const;
	const class ALadder* GetCurrentLadder();

protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	void PhysMantling(float DeltaTime, int32 Iterations);

	void PhysLadder(float DeltaTime, int32 Iterations);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(Category = "Character Movement: Swimming", EditAnywhere,BlueprintReadOnly, meta = (ClampMin="0", UIMin="0"))
	float SwimmingCapsuleRadius = 60.0f;

	UPROPERTY(Category = "Character Movement: Swimming", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float SwimmingCapsuleHalfHeight = 60.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float ClimbingOnLadderMaxSpeed = 100.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float ClimbingOnLadderBreakingDecelaration = 2048.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float LadderToCharacterOffset = 60.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MaxLadderTopOffset = 90.0f;

	UPROPERTY(Category = "Character Movement: Ladder", EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float MinLadderBottomOffset = 90.0f;

	class AXYZBaseCharacter* GetBaseCharacterOwner() const;


private:
	bool bIsSprinting;
	bool bIsOutOfStamina;
	bool bIsMantling = false;

	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;

	const ALadder* CurrentLadder = nullptr;
};
