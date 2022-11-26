// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XYZTypes.h"
#include "Actors/Equipment/Throwables/ThrowableItem.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<int32, TInlineAllocator<(uint32)EAmunitionType::MAX>> TAmunitionArray;
typedef TArray<class AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrenWeaponAmmoChanged, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedItemChanged, const AEquipableItem*);

class AThrowableItem;
class ARangeWeaponItem;
class UEquipmentViewWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XYZ_CHARACTER_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquipableItemType GetCurrentEquippedItemType() const;

	ARangeWeaponItem* GetCurrentRangeWeapon() const;

	FOnCurrenWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;

	FOnEquippedItemChanged OnEquippedItemChanged;

	void ReloadCurrentWeapon();

	void ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo = 0, bool bCheckIsFool = false);

	void EquipItemInSlot(EEquipmentSlots Slot);

	void AttachCurrentItemToEquippedSocket();

	void UnEquipCurrentItem();

	void EquipNextItem();
	void EquipPreviousItem();

	bool IsEquipping() const;

	void LaunchCurrentThrowableItem();

	bool AddEquipmentItemToSlot(const TSubclassOf<AEquipableItem> EquipableItemClass, int32 SlotIndex);
	void RemoveItemFromSlot(int32 SlotIndex);

	void OpenViewEquipment(APlayerController* PlayerController);
	void CloseViewEquipment();
	bool IsViewVisible() const;

	const TArray<AEquipableItem*>& GetItems() const;

protected:
		
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EAmunitionType, int32> MaxAmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TMap<EEquipmentSlots, TSubclassOf<class AEquipableItem>> ItemsLoadout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout")
	TSet<EEquipmentSlots> IgnoreSlotsWhileSwitching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loadout")
	EEquipmentSlots AutoEquipItemInSlot = EEquipmentSlots::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View")
	TSubclassOf<UEquipmentViewWidget> ViewWidgetClass;

	void CreateViewWidget(APlayerController* PlayerController);


private:
	TAmunitionArray AmunitionArray;
	TArray<AEquipableItem*> ItemsArray;

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);

	bool bIsEquipping = false;

	void EquipAnimationFinished();

	void AutoEquip();

	UFUNCTION()
	void OnWeaponReloadComplete();

	int32 GetAvailableAmunitionForCurrentWeapon();

	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 Ammo);

	FDelegateHandle OnCurrentWeaponAmmoChangeHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	EEquipmentSlots PreviousEquippedSlot;

	AEquipableItem* CurrentEquippedItem;
	EEquipmentSlots CurrentEquippedSlot;
	AThrowableItem* CurrentThrowableItem;

	FTimerHandle EquipTimer;

	void CreateLoadout();

	class ARangeWeaponItem* CurrentEquippedWeapon;

	TWeakObjectPtr<class AXYZBaseCharacter> CachedBaseCharacter;

	UEquipmentViewWidget* ViewWidget;
};
