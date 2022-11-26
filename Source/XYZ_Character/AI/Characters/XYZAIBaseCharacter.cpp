// Fill out your copyright notice in the Description page of Project Settings.


#include "XYZAIBaseCharacter.h"
#include "Characters/Components/CharacterComponents/AIPatrollingComponent.h"
#include "AIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

AXYZAIBaseCharacter::AXYZAIBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrolling"));
}

UAIPatrollingComponent* AXYZAIBaseCharacter::GetPatrollingComponent() const
{
	return AIPatrollingComponent;
}

UBehaviorTree* AXYZAIBaseCharacter::GetBehaviorTree() const
{
		return BehaviorTree;
}
