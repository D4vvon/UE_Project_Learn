// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/XYZBaseCharacter.h"
#include "XYZAIBaseCharacter.generated.h"

class UBehaviorTree;
class UAIPatrollingComponent;
UCLASS(Blueprintable)
class XYZ_CHARACTER_API AXYZAIBaseCharacter : public AXYZBaseCharacter
{
	GENERATED_BODY()

public:
	AXYZAIBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UAIPatrollingComponent* GetPatrollingComponent() const;

	UBehaviorTree* GetBehaviorTree() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAIPatrollingComponent* AIPatrollingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
};
