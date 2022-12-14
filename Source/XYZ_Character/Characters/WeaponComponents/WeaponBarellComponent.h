// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NiagaraSystem.h"
#include "WeaponBarellComponent.generated.h"

UENUM(BlueprintType)
enum class EHitRegistrationType : uint8
{
	HitScan,
	Projectile
};

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	FVector DecalSize = FVector(5.0f, 5.0f, 5.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	float DecalLifeTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Decal info")
	float DecalFadeOutTime = 5.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XYZ_CHARACTER_API UWeaponBarellComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	
	void Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	float FiringRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Barell attributes | Damage")
	float DamgeAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon | Barell attributes | Damage")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | VFX")
	UNiagaraSystem* MuzzleFlashFX;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Barell attributes | VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Decals")
	FDecalInfo DefaultDecalInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes", meta = (ClampMin = 1, UIMin = 1))
	int32  BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration")
	EHitRegistrationType HitRegistration = EHitRegistrationType::HitScan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration", meta = (EditCondition = "HitRegistration = EHitRegistrationType::Projectile"))
	TSubclassOf<class AXYZProjectile> ProjectileClass;

private:
	APawn* GetOwningPawn() const;
	AController* GetController() const;

	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& Direction);

	bool HitScan(FVector ShotStart, OUT FVector& ShotEnd, FVector ShotDirection);
	void LounchProjectile(const FVector& LounchStart, const FVector& LounchDirection);

	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const;
};
