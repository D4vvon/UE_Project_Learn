// Fill out your copyright notice in the Description page of Project Settings.


#include "PickablePowerups.h"
#include "../../../XYZTypes.h"
#include "../../../Characters/XYZBaseCharacter.h"
#include "../../../Inventory/Items/InventoryItem.h"
#include "../../../Utils/DataTable/GameDataTableUtils.h"

using namespace GameDataTableUtils;

APickablePowerups::APickablePowerups()
{
	PoweupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMesh"));
	SetRootComponent(PoweupMesh);
}

void APickablePowerups::Interact(AXYZBaseCharacter* Character)
{
	FItemTableRow* ItemData = GameDataTableUtils::FindInventoryItemData(GetDataTableID());

	if (ItemData == nullptr)
	{
		return;
	}

	TWeakObjectPtr<UInventoryItem> Item = TWeakObjectPtr<UInventoryItem>(NewObject<UInventoryItem>(Character, ItemData->InventoryItemClass));
	Item->Initialize(DataTableID, ItemData->InventoryItemDescription);

	const bool bPickedUp = Character->PickupItem(Item);
	if (bPickedUp == true)
	{
		Destroy();
	}
}

FName APickablePowerups::GetActionEventName() const
{
	return ActionInteract;
}
