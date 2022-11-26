// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XYZAIController.h"
#include "AITurretController.generated.h"

class ATurret;
UCLASS()
class XYZ_CHARACTER_API AAITurretController : public AXYZAIController
{
	GENERATED_BODY()

public:

	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:

	TWeakObjectPtr<ATurret> CachedTurret;
	
};
