// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Fire.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Characters/XYZBaseCharacter.h"
#include "../../Characters/Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "../../Actors/Equipment/Weapons/RangeWeaponItem.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(AIController) || !IsValid(BlackboardComp))
	{
		return;
	}

	AXYZBaseCharacter* Character = Cast<AXYZBaseCharacter>(AIController->GetPawn());
	if (!IsValid(Character))
	{
		return;
	}

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();
	ARangeWeaponItem* RangeWeapon = EquipmentComponent->GetCurrentRangeWeapon();
	if (!IsValid(RangeWeapon))
	{
		return;
	}

	AActor* CurrentTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(CurrentTarget))
	{
		Character->StopFire();
		return;
	}

	float DistSquared = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());
	if (DistSquared > FMath::Square(MaxFireDistance))
	{
		Character->StopFire();
		return;
	}

	if (!RangeWeapon->IsReloading() || !RangeWeapon->IsFiring())
	{
		Character->StartFire();
	}
}
