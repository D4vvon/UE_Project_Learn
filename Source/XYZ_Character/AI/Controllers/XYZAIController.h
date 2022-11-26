// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "XYZAIController.generated.h"

class UAISense;
UCLASS()
class XYZ_CHARACTER_API AXYZAIController : public AAIController
{
	GENERATED_BODY()

public:
	AXYZAIController();

protected:
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const;


	
};
