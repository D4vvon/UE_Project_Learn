// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactive/Pickables/PickableItem.h"
#include "../Interface/Interactable.h"
#include "Utils/DataTable/GameDataTableUtils.h"
#include "PickableWeapon.generated.h"

using namespace GameDataTableUtils;

class AXYZBaseCharacter;
UCLASS(Blueprintable)
class XYZ_CHARACTER_API APickableWeapon : public APickableItem
{
	GENERATED_BODY()
	
public:
	APickableWeapon();

	virtual void Interact(AXYZBaseCharacter* Character) override;
	virtual FName GetActionEventName() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;
};
