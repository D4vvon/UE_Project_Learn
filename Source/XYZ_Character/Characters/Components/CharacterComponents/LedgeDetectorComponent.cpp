#include "LedgeDetectorComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "../../../XYZTypes.h"
#include "DrawDebugHelpers.h"
#include "../../../Utils/XYZTraceUtils.h"
#include "../../XYZBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../../XYZGameInstance.h"
#include "../../../Subsystems/DebugSubsystem.h"


void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<ACharacter>(), TEXT("ULedgeDetectorComponent::BeginPlay() only a character can use ULedgeDetector"));
	ChachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
	
}

bool ULedgeDetectorComponent::DetectLedge(OUT FLedgeDescription& LedgeDescription)
{
	UCapsuleComponent* CapsuleComponent = ChachedCharacterOwner->GetCapsuleComponent();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

#if ENABLE_DRAW_DEBUG
	UDebugSubsystem* DebugSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDebugSubsystem>();
	bool bIsDebugEnabled = DebugSubsystem->IsCategoryEnabled(DebugCategoryLedgeDetection);
#else
	bool bIsDebugEnabled = false;
#endif

	float BottomZOffset = 2.0f;// offset for avoiding the error of coincidence of player's collision and floor
	FVector CharacterBottom = ChachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;

	//1.Forward check

	float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5;

	FHitResult ForwardCheckHitResult;
	//FCollisionShape ForwardCapsuleShape = FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + ChachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;


	float DrawTime = 2.0f;

	if (!XYZTraceUtils::SweepCapsuleSingleByChanel(GetWorld(),ForwardCheckHitResult,ForwardStartLocation,ForwardEndLocation,ForwardCheckCapsuleRadius,ForwardCheckCapsuleHalfHeight, FQuat::Identity, ECC_Climbing, QueryParams,FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	//replace to XYZTraceUtils
	//DrawDebugCapsule(GetWorld(), ForwardStartLocation, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Black, false, DrawTime);
	//DrawDebugCapsule(GetWorld(), ForwardEndLocation, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Black, false, DrawTime);
	//DrawDebugLine(GetWorld(), ForwardStartLocation, ForwardEndLocation, FColor::Black, false, DrawTime);

	//if (!GetWorld()->SweepSingleByChannel(ForwardCheckHitResult, ForwardStartLocation, ForwardEndLocation, FQuat::Identity, ECC_Climbing, ForwardCapsuleShape, QueryParams))//RayCast to find obstacle
	//{
	//	return false;
	//}
	//DrawDebugCapsule(GetWorld(), ForwardCheckHitResult.Location, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Red, false, DrawTime);
	//DrawDebugPoint(GetWorld(), ForwardCheckHitResult.ImpactPoint, 10.0f, FColor::Red, false, DrawTime);
	//replace to XYZTraceUtils


	//2.Downward check

	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float DownwardCheckDepthOffset = 10.0f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	if (!XYZTraceUtils::SweepSphereSingleByChanel(GetWorld(), DownwardCheckHitResult, DownwardStartLocation, DownwardEndLocation, DownwardSphereCheckRadius, ECC_Climbing, QueryParams, FCollisionResponseParams::DefaultResponseParam, bIsDebugEnabled, DrawTime))
	{
		return false;
	}
	//3.Overlap check

	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	float OverlapCapsileFloorOffset = 2.0f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + OverlapCapsileFloorOffset) *FVector::UpVector;

	if (XYZTraceUtils::OverlapCapsuleAnyByProfile(GetWorld(), OverlapLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight, FQuat::Identity, CollisionProfilePawn, QueryParams, bIsDebugEnabled, DrawTime))
	{
		return false;
	}

	LedgeDescription.Location = OverlapLocation;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.0f, -1.0f, 0.0f)).ToOrientationRotator();
	LedgeDescription.LedgeNormal = ForwardCheckHitResult.ImpactNormal;

	return true;
}


