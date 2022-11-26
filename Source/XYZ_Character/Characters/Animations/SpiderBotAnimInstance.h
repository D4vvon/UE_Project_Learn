// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCBasePawnAnimInstance.h"
#include "../SpiderPawn.h"
#include "SpiderBotAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYZ_CHARACTER_API USpiderBotAnimInstance : public UGCBasePawnAnimInstance
{
	GENERATED_BODY()
	
public:
		virtual void NativeBeginPlay() override;
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	protected:
		UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Spider Bot|IK settings")
		FVector RightFrontFootEffectorLocation = FVector::ZeroVector;
	
		UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Spider Bot|IK settings")
		FVector RightRearFootEffectorLocation = FVector::ZeroVector;
	
		UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Spider Bot|IK settings")
		FVector LeftFrontFootEffectorLocation = FVector::ZeroVector;
	
		UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly, Category = "Spider Bot|IK settings")
		FVector LeftRearFootEffectorLocation = FVector::ZeroVector;
		
	
	private:
		TWeakObjectPtr<class ASpiderPawn> CachedSpiderPawnOwner;
};
