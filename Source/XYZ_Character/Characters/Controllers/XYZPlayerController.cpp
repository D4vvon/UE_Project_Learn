// Fill out your copyright notice in the Description page of Project Settings.


#include "XYZPlayerController.h"
#include "../XYZBaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/ReticleWidget.h"
#include "UI/Widgets/PlayerHUDWidget.h"
#include "UI/Widgets/AmmoWidget.h"
#include "../Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "XYZTypes.h"
#include "GameFramework/PlayerInput.h"

void AXYZPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBasedCharacter = Cast<AXYZBaseCharacter>(InPawn);
	if (CachedBasedCharacter.IsValid() && IsLocalController())
	{
		CreateAndInitiolizeWidgets();
		CachedBasedCharacter->OnInteractableObjectFound.BindUObject(this, &AXYZPlayerController::OnInteractableObjectFound);
	}
}

void AXYZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AXYZPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AXYZPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AXYZPlayerController::Turn);
	InputComponent->BindAxis("TurnAtRate", this, &AXYZPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AXYZPlayerController::LookUp);
	InputComponent->BindAxis("LookUpAtRate", this, &AXYZPlayerController::LookUpAtRate);
	InputComponent->BindAction("InteractWithLadder", EInputEvent::IE_Pressed, this, &AXYZPlayerController::InteractiveWithLadder);
	InputComponent->BindAction("Mantle", EInputEvent::IE_Pressed, this, &AXYZPlayerController::Mantle);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AXYZPlayerController::Jump);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AXYZPlayerController::ChangeCrouchState);

	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AXYZPlayerController::StartSprint);
	InputComponent->BindAction("StopSprint", EInputEvent::IE_Released, this, &AXYZPlayerController::StopSprint);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AXYZPlayerController::PlayerStartFire);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AXYZPlayerController::PlayerStopFire);
	InputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &AXYZPlayerController::StartAiming);
	InputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &AXYZPlayerController::StopAiming);
	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AXYZPlayerController::Reload);
	InputComponent->BindAction("NextItem", EInputEvent::IE_Pressed, this, &AXYZPlayerController::NextItem);
	InputComponent->BindAction("PreviousItem", EInputEvent::IE_Pressed, this, &AXYZPlayerController::PreviousItem);

	InputComponent->BindAction("EquipPrimatyItem", EInputEvent::IE_Pressed, this, &AXYZPlayerController::EquipPrimaryItem);

	InputComponent->BindAction(ActionInteract, EInputEvent::IE_Pressed, this, &AXYZPlayerController::Interact);
	InputComponent->BindAction("UseInventory", EInputEvent::IE_Pressed, this, &AXYZPlayerController::UseInventory);

	InputComponent->BindAxis("SwimForward", this, &AXYZPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AXYZPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AXYZPlayerController::SwimUp);
	InputComponent->BindAxis("ClimbLadderUp", this, &AXYZPlayerController::ClimbLadderUp);
}

void AXYZPlayerController::OnInteractableObjectFound(FName ActionName)
{
	if (!IsValid(PlayerHUDWidget))
	{
		return;
	}

	TArray<FInputActionKeyMapping> ActionKeys = PlayerInput->GetKeysForAction(ActionName);
	const bool HasAnyKeys = ActionKeys.Num() != 0;
	if (HasAnyKeys)
	{
		FName ActionKey = ActionKeys[0].Key.GetFName();
		PlayerHUDWidget->SetHighLightInteractableActionText(ActionKey);
	}
	PlayerHUDWidget->SetHighLightInteractableVisibility(HasAnyKeys);
}

void AXYZPlayerController::MoveForward(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		if (!CachedBasedCharacter->IsOutOfStamina())
		{
			Value /= 3;
		}
		CachedBasedCharacter->MoveForward(Value);
	}
}

void AXYZPlayerController::MoveRight(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		if (!CachedBasedCharacter->IsOutOfStamina())
		{
			Value /= 3;
		}
		CachedBasedCharacter->MoveRight(Value);
	}
}

void AXYZPlayerController::Turn(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->Turn(Value);
	}
}

void AXYZPlayerController::LookUp(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->LookUp(Value);
	}
}

void AXYZPlayerController::TurnAtRate(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->TurnAtRate(Value);
	}
}

void AXYZPlayerController::LookUpAtRate(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->LookUpAtRate(Value);
	}
}

void AXYZPlayerController::ChangeCrouchState()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->ChangeCrouchState();
	}
}

void AXYZPlayerController::Mantle()
{
	if (CachedBasedCharacter.IsValid() && !CachedBasedCharacter->GetCharacterMovement()->IsCrouching())
	{
		CachedBasedCharacter->Mantle();
	}
}

void AXYZPlayerController::Jump()
{
	if (CachedBasedCharacter->IsOutOfStamina() && !CachedBasedCharacter->GetBaseCharacterMovementComponent()->IsFalling())
	{
		if (CachedBasedCharacter.IsValid())
		{
			CachedBasedCharacter->Jump();

		}
	}
}

void AXYZPlayerController::StartSprint()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->StartSprint();
	}
}

void AXYZPlayerController::StopSprint()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->StopSprint();
	}
}

void AXYZPlayerController::SwimForward(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->SwimForward(Value);
	}
}

void AXYZPlayerController::SwimRight(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->SwimRight(Value);
	}
}

void AXYZPlayerController::SwimUp(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->SwimUp(Value);
	}
}

void AXYZPlayerController::PlayerStartFire()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->StartFire();
	}
}

void AXYZPlayerController::PlayerStopFire()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->StopFire();
	}
}

void AXYZPlayerController::StartAiming()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->StartAiming();
	}
}

void AXYZPlayerController::StopAiming()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->StopAiming();
	}
}

void AXYZPlayerController::NextItem()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->NextItem();
	}
}

void AXYZPlayerController::PreviousItem()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->PreviousItem();
	}
}

void AXYZPlayerController::Reload()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->Reload();
	}
}

void AXYZPlayerController::ClimbLadderUp(float Value)
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->ClimbLadderUp(Value);
	}
}

void AXYZPlayerController::InteractiveWithLadder()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->InteractiveWithLadder();
	}
}

void AXYZPlayerController::EquipPrimaryItem()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->EquipPrimaryItem();
	}
}

void AXYZPlayerController::Interact()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->Interact();
	}
}

void AXYZPlayerController::UseInventory()
{
	if (CachedBasedCharacter.IsValid())
	{
		CachedBasedCharacter->UseInventory(this);
	}
}

void AXYZPlayerController::CreateAndInitiolizeWidgets()
{
	if (!IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);

		if (IsValid(PlayerHUDWidget))
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (CachedBasedCharacter.IsValid() && IsValid(PlayerHUDWidget))
	{
		UReticleWidget* ReticleWidget = PlayerHUDWidget->GetReticleWidget();
			if (IsValid(ReticleWidget))
			{
				CachedBasedCharacter->OnAimingStateChanged.AddUFunction(ReticleWidget, FName("OnAimingStateChange"));
				UCharacterEquipmentComponent* CharacterEquipment = CachedBasedCharacter->GetCharacterEquipmentComponent_Mutable();
				CharacterEquipment->OnEquippedItemChanged.AddUFunction(ReticleWidget, FName("OnEquippedItemChanged"));
			}
			
			UAmmoWidget* AmmoWidget = PlayerHUDWidget->GetAmmoWidget();
			if (IsValid(AmmoWidget))
			{
				UCharacterEquipmentComponent* CharacterEquipment = CachedBasedCharacter->GetCharacterEquipmentComponent_Mutable();
				CharacterEquipment->OnCurrentWeaponAmmoChangedEvent.AddUFunction(AmmoWidget, FName("UpdateAmmoCount"));
			}
	}

}
