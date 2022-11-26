// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Actors/Interactive/Pickables/PickableItem.h"
#include "Actors/Equipment/EquipableItem.h"
#include "InventoryItem.generated.h"

class UInventoryItem;
class APickableItem;
class AEquipableItem;
class AXYZBaseCharacter;

USTRUCT(BlueprintType)
struct FInvetoryItemDesription : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon view")
	TSubclassOf<APickableItem> PickableActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon view")
	TSubclassOf<AEquipableItem> EquipableActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon view")
	FInvetoryItemDesription WeaponItemDescription;
};

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	TSubclassOf<APickableItem> PickableActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	TSubclassOf<UInventoryItem> InventoryItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item view")
	FInvetoryItemDesription InventoryItemDescription;


};

UCLASS(Blueprintable)
class XYZ_CHARACTER_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(FName DataTableID_In, const FInvetoryItemDesription& Description_In);

	FName GetDataTableID() const;
	const FInvetoryItemDesription& GetDescription() const;

	virtual bool IsEquipable() const;
	virtual bool IsConsumable() const;

	virtual bool Consume(AXYZBaseCharacter* ConsumeTarget) PURE_VIRTUAL(UInventoryItem::Consume(), return false;);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventoryItem")
	FName DataTableID = NAME_None;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Inventory Item")
	FInvetoryItemDesription Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Item")
	bool bIsEquipable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Item")
	bool bIsConsumable = false;

private:
	bool bIsInitialized = false;
};
