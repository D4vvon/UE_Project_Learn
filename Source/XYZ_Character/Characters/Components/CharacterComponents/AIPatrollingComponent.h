// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIPatrollingComponent.generated.h"

class APatrollingPath;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XYZ_CHARACTER_API UAIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	bool CanPatroll() const;

	FVector SelectClosestWayPoint();
	FVector SelectNextWayPoint();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Path")
	APatrollingPath* PatrollingPath;

private:
	int32 CurrentWayPointIndex = -1;
};
