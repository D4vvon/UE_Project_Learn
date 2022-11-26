// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

void UInventoryItem::Initialize(FName DataTableID_In, const FInvetoryItemDesription& Description_In)
{
	DataTableID = DataTableID_In;
	Description.Icon = Description_In.Icon;
	Description.Name = Description_In.Name;
	bIsInitialized = true;
}

FName UInventoryItem::GetDataTableID() const
{
	return DataTableID;
}

const FInvetoryItemDesription& UInventoryItem::GetDescription() const
{
	return Description;
}

bool UInventoryItem::IsEquipable() const
{
	return bIsEquipable;
}

bool UInventoryItem::IsConsumable() const
{
	return bIsConsumable;
}
