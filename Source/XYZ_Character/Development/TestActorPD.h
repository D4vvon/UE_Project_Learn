// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActorPD.generated.h"

UCLASS()
class XYZ_CHARACTER_API ATestActorPD : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActorPD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetRandomRotationSpeed();

	UPROPERTY(EditAnywhere)
	float MinCubeRotationSpeed = 50.0f;

	UPROPERTY(EditAnywhere)
	float MaxCubeRotationSpeed = 500.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	float CurrentRotationSpeed = 0.0f;
};
