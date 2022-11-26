// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"
#include "Characters/WeaponComponents/WeaponBarellComponent.h"
#include "XYZTypes.h"
#include "Characters/XYZBaseCharacter.h"
#include "GameFramework/PlayerController.h"

ARangeWeaponItem::ARangeWeaponItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponBarell = CreateDefaultSubobject<UWeaponBarellComponent>(TEXT("WeaponBarell"));
	WeaponBarell->SetupAttachment(WeaponMesh, SocketWeaponWuzzle);

	ReticleType = EReticleType::Default;

	EquippedSocketName = SocketCharacterWeapon;
}

void ARangeWeaponItem::StartFire()
{
	bIsFiring = true;
	MakeShot();
	if (WeaponFireMode == EWeaponFireMode::FullAuto)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ARangeWeaponItem::MakeShot, GetShotTimerInterval(), true);
	}
}

void ARangeWeaponItem::StopFire()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
}

void ARangeWeaponItem::StartAim()
{
	bIsAiming = true;
}

void ARangeWeaponItem::StopAim()
{
	bIsAiming = false;
}

bool ARangeWeaponItem::IsFiring() const
{
	return bIsFiring;
}

bool ARangeWeaponItem::IsReloading() const
{
	return bIsReloading;
}

float ARangeWeaponItem::GetAimFOV() const
{
	return AimFOV;
}

float ARangeWeaponItem::GetAimMovementMaxSpeed() const
{
	return AimMovementMaxSpeed;
}

int32 ARangeWeaponItem::GetAmmo() const
{
	return Ammo;
}

int32 ARangeWeaponItem::GetMaxAmmo() const
{
	return MaxAmmo;
}

EAmunitionType ARangeWeaponItem::GetAmmoType() const
{
	return AmmoType;
}

void ARangeWeaponItem::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
	if (OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(Ammo);
	}
}

bool ARangeWeaponItem::CanShoot() const
{
	if (Ammo > 0 && !bIsReloading)
	{
		return true;
	}
	else
	{
		return false;
	}
	//return Ammo > 0;
}

void ARangeWeaponItem::StartReload()
{
	checkf(GetOwner()->IsA<AXYZBaseCharacter>(), TEXT("ARangeWeaponItem::StartReload() only character can be an owner of range weapon"));
	AXYZBaseCharacter* CharacterOwner = StaticCast<AXYZBaseCharacter*>(GetOwner());

	bIsReloading = true;
	if (IsValid(CharacterReloadMontage))
	{
		float MontageDuration =  CharacterOwner->PlayAnimMontage(CharacterReloadMontage);
		PlayAnimMontage(WeaponReloadMontage);
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer,[this]() { EndReload(true); }, MontageDuration, false);
	}
	else
	{
		EndReload(true);
	}

}

void ARangeWeaponItem::EndReload(bool bIsSuccess, bool bJumpToEnd /*= false*/)
{

	if (!bIsReloading)
	{
		return;
	}
	if (!bIsSuccess)
	{
		checkf(GetOwner()->IsA<AXYZBaseCharacter>(), TEXT("ARangeWeaponItem::StartReload() only character can be an owner of range weapon"));
		AXYZBaseCharacter* CharacterOwner = StaticCast<AXYZBaseCharacter*>(GetOwner());
		CharacterOwner->StopAnimMontage(CharacterReloadMontage);
		StopAnimMontage(WeaponReloadMontage, false);
	}

	if (bJumpToEnd)
	{
		AXYZBaseCharacter* CharacterOwner = StaticCast<AXYZBaseCharacter*>(GetOwner());
		UAnimInstance* CharacterAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
		if (IsValid(CharacterAnimInstance))
		{
			CharacterAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, CharacterReloadMontage);
		}

		UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
		if (IsValid(WeaponAnimInstance))
		{
			WeaponAnimInstance->Montage_JumpToSection(SectionMontageReloadEnd, WeaponReloadMontage);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);

	bIsReloading = false;
	if (bIsSuccess && OnReloadComplete.IsBound())
	{
		OnReloadComplete.Broadcast();
	}
}

FTransform ARangeWeaponItem::GetForeGripTransform() const
{
	return WeaponMesh->GetSocketTransform(SocketWeaponForeGrip);
}

EReticleType ARangeWeaponItem::GetReticleType() const
{
	return bIsAiming ? AimReticleType : ReticleType;
}

void ARangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	SetAmmo(MaxAmmo);
}

float ARangeWeaponItem::GetShotTimerInterval() const
{
	return 60.0f / RateOfFire;
}

float ARangeWeaponItem::GetCurrentBulletSpread() const
{
	float AngleInDegress = bIsAiming ? AimSpreadAngle : SpreadAngle;
	return FMath::DegreesToRadians(AngleInDegress);
}

float ARangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	float Result = 0.0f;
	if (IsValid(WeaponAnimInstance))
	{
		Result = WeaponAnimInstance->Montage_Play(AnimMontage);
	}
	return Result;
}

void ARangeWeaponItem::StopAnimMontage(UAnimMontage* AnimMontage, float BleandOutTime /*= 0*/)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (IsValid(WeaponAnimInstance))
	{
		WeaponAnimInstance->Montage_Stop(BleandOutTime, AnimMontage);
	}
}

void ARangeWeaponItem::MakeShot()
{
	checkf(GetOwner()->IsA<AXYZBaseCharacter>(), TEXT("ARangeWeaponItem::Fire() only character can be an owner of range weapon"));
	AXYZBaseCharacter* CharacterOwner = StaticCast<AXYZBaseCharacter*>(GetOwner());

	if (!CanShoot())
	{
		StopFire();
		if (Ammo == 0 && bAutoReload)
		{
			CharacterOwner->Reload();
		}
		return;
	}

	EndReload(false);

	CharacterOwner->PlayAnimMontage(CharacterFireMotage);
	PlayAnimMontage(WeaponFireMotage);

	FVector ShotLocation;
	FRotator ShotRotation;

	if (CharacterOwner->IsPlayerControlled())
	{
		APlayerController* Controller = CharacterOwner->GetController<APlayerController>();
		Controller->GetPlayerViewPoint(ShotLocation, ShotRotation);
	}
	else
	{
		ShotLocation = WeaponBarell->GetComponentLocation();
		ShotRotation = CharacterOwner->GetBaseAimRotation();
	}

	FVector ShotDirection = ShotRotation.RotateVector(FVector::ForwardVector);

	SetAmmo(Ammo - 1);
	WeaponBarell->Shot(ShotLocation, ShotDirection, GetCurrentBulletSpread());
}
