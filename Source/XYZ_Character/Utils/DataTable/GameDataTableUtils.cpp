#include "GameDataTableUtils.h"
#include "Inventory/Items/InventoryItem.h"
#include "Engine/DataTable.h"

FWeaponTableRow* GameDataTableUtils::FindWeaponData(const FName WeaponID)
{
	static const FString ContextString(TEXT("Find Weapon Data"));

	UDataTable* WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/GameCode/Core/Data/DataTables/DT_WeaponList.DT_WeaponList"));

	if (WeaponDataTable == nullptr)
	{
		return nullptr;
	}

	return WeaponDataTable->FindRow<FWeaponTableRow>(WeaponID, ContextString);
}

FItemTableRow* GameDataTableUtils::FindInventoryItemData(const FName ItemID)
{
	static const FString ContextString(TEXT("Find Item Data"));

	///Game/Data/DataTables/DT_PickableWeapon.DT_PickableWeapon
	UDataTable* InventoryItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/GameCode/Core/Data/DataTables/DT_InvenotryItemList.DT_InvenotryItemList"));

	if (InventoryItemDataTable == nullptr)
	{
		return nullptr;
	}

	return InventoryItemDataTable->FindRow<FItemTableRow>(ItemID, ContextString);
}
