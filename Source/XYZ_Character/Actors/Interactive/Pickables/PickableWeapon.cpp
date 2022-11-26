// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "XYZTypes.h"
#include "Utils/DataTable/GameDataTableUtils.h"
#include "Inventory/Items/InventoryItem.h"
#include "Characters/XYZBaseCharacter.h"
#include "Inventory/Items/Equipables/WeaponInventoryItem.h"

APickableWeapon::APickableWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void APickableWeapon::Interact(AXYZBaseCharacter* Character)
{
	FWeaponTableRow* WeaponRow = GameDataTableUtils::FindWeaponData(DataTableID);
	if (WeaponRow)
	{
		//Character->AddEquipmentItem(WeaponRow->EquipableActor); // Add on Equipped slot
		TWeakObjectPtr<UWeaponInventoryItem> Weapon = NewObject<UWeaponInventoryItem>(Character);
		Weapon->Initialize(DataTableID, WeaponRow->WeaponItemDescription);
		Weapon->SetEquipWeaponClass(WeaponRow->EquipableActor);
		Character->PickupItem(Weapon); // add on inventory slot
		Destroy();
	}
}

FName APickableWeapon::GetActionEventName() const
{
	return ActionInteract;
}
