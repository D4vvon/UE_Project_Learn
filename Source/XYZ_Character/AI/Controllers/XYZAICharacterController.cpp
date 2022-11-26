// Fill out your copyright notice in the Description page of Project Settings.


#include "XYZAICharacterController.h"
#include "AI/Characters/XYZAIBaseCharacter.h"
#include "Perception/AISense_Sight.h"
#include "Characters/Components/CharacterComponents/AIPatrollingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "XYZTypes.h"

void AXYZAICharacterController::SetPawn(APawn* InPawn)
{ 
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<AXYZAIBaseCharacter>(), TEXT("AXYZAICharacterController::SetPawn AICharacterController can possess only XYZAIBaseCharacter"));
		ChachedAICharacter = StaticCast<AXYZAIBaseCharacter*>(InPawn);
		RunBehaviorTree(ChachedAICharacter->GetBehaviorTree());
		SetupPatrolling();
	}
	else
	{
		ChachedAICharacter = nullptr;
	}
}

void AXYZAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if (!ChachedAICharacter.IsValid())
	{
		return;
	}

	TryMoveToNextTarget();
}

void AXYZAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
	{
		return;
	}
	TryMoveToNextTarget();
}

void AXYZAICharacterController::SetupPatrolling()
{
	UAIPatrollingComponent* PatrollingComponent = ChachedAICharacter->GetPatrollingComponent();
	if (PatrollingComponent->CanPatroll())
	{
		FVector CloasestWayPoint = PatrollingComponent->SelectClosestWayPoint();
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, CloasestWayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

void AXYZAICharacterController::TryMoveToNextTarget()
{
	UAIPatrollingComponent* PatrollingComponent = ChachedAICharacter->GetPatrollingComponent();

	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsObject(BB_CurrentTarget, ClosestActor);
			SetFocus(ClosestActor, EAIFocusPriority::Gameplay);
		}
		bIsPatrolling = false;
	}
	else if(PatrollingComponent->CanPatroll())
	{
		FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNextWayPoint() : PatrollingComponent->SelectClosestWayPoint();
		if (IsValid(Blackboard))
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

bool AXYZAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - ChachedAICharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachRadius);
}
