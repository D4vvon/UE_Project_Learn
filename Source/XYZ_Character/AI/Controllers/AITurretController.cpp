// Fill out your copyright notice in the Description page of Project Settings.


#include "AITurretController.h"
#include "Perception/AIPerceptionComponent.h"
#include "../Characters/Turret.h"
#include "Perception/AISense_Sight.h"

void AAITurretController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ATurret>(), TEXT("AAITurretController::SetPawn AAITurretController can posses only Turrets"));
		CachedTurret = StaticCast<ATurret*>(InPawn);
	}
	else
	{
		CachedTurret = nullptr;
	}
}

void AAITurretController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!CachedTurret.IsValid())
	{
		return;
	}

	//TArray<AActor*> SeenActors;
	//PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeenActors);

	//AActor* ClosestActor = nullptr;
	//float MinSquaredDistance = FLT_MAX;

	//FVector TurretLocation = CachedTurret->GetActorLocation();

	//for (AActor* SeenActor : SeenActors)
	//{
	//	float CurrentSquaredDistance = (TurretLocation - SeenActor->GetActorLocation()).SizeSquared(); // equal to next str;
	//	FVector::DistSquared(TurretLocation, SeenActor->GetActorLocation());
	//	if (CurrentSquaredDistance < MinSquaredDistance)
	//	{
	//		MinSquaredDistance = CurrentSquaredDistance;
	//		ClosestActor = SeenActor;
	//	}
	//}

	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());

	CachedTurret->SetCurrentTarget(ClosestActor);
}
