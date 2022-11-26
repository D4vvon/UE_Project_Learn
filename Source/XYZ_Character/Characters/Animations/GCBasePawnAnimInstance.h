// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GCBasePawnAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYZ_CHARACTER_API UGCBasePawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:


	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn animation istanse")
	float InputForward = 0.0f;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn animation istanse")
	float InputRight = 0.0f;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base pawn animation istanse")
	bool bIsInAir;



private:

	TWeakObjectPtr<class AGameCodeBasePawn> CachedBasePawn;
};
