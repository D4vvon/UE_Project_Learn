// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ASpiderPawn::ASpiderPawn()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Spider mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	IKScale = GetActorScale3D().Z;
	IKTraceDistance = CollisionSphereRadius * IKScale;
}

void ASpiderPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	IKRightFrontFootOffset = GetIKOffsetForASoket(RightFrontFootSocketName);
	IKRightRearFootOffset = GetIKOffsetForASoket(RightRearFootSocketName);
	IKLeftFrontFootOffset = GetIKOffsetForASoket(LeftFrontFootSocketName);
	IKLeftRearFootOffset = GetIKOffsetForASoket(LeftRearFootSocketName);
}

float ASpiderPawn::GetIKOffsetForASoket(const FName& SoketName)
{
	float Result = 0.0f;

	FVector SoketLocation = SkeletalMeshComponent->GetSocketLocation(SoketName);
	FVector TraceStart(SoketLocation.X, SoketLocation.Y, GetActorLocation().Z);
	FVector TraceEnd = TraceStart - IKTraceDistance * FVector::UpVector;


	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		Result = (TraceEnd.Z - HitResult.Location.Z) / IKScale;
	}

	return Result;
}
