// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActorPD.h"

// Sets default values
ATestActorPD::ATestActorPD()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActorPD::BeginPlay()
{
	Super::BeginPlay();
	
	SetRandomRotationSpeed();
}

void ATestActorPD::SetRandomRotationSpeed()
{
	CurrentRotationSpeed = FMath::RandRange(MinCubeRotationSpeed, MaxCubeRotationSpeed);
	GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Current speed: %f"), CurrentRotationSpeed));
}

// Called every frame
void ATestActorPD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += CurrentRotationSpeed * DeltaTime;
	SetActorRotation(Rotation);

}

