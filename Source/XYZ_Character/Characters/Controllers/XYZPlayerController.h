// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XYZPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class XYZ_CHARACTER_API AXYZPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

private:

	void OnInteractableObjectFound(FName ActionName);

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void ChangeCrouchState();
	void Mantle();
	void Jump();

	void StartSprint();
	void StopSprint();

	void SwimForward(float Value);
	void SwimRight(float Value);
	void SwimUp(float Value);

	void PlayerStartFire();
	void PlayerStopFire();

	void StartAiming();
	void StopAiming();

	void NextItem();
	void PreviousItem();

	void Reload();

	void ClimbLadderUp(float Value);
	void InteractiveWithLadder();

	void EquipPrimaryItem();

	void Interact();

	void UseInventory();

	UPlayerHUDWidget* PlayerHUDWidget = nullptr;

	void CreateAndInitiolizeWidgets();


	TSoftObjectPtr<class AXYZBaseCharacter> CachedBasedCharacter;
};
