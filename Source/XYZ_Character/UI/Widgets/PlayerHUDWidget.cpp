// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Characters/XYZBaseCharacter.h"
#include "Characters/Components/CharacterComponents/CharacterAttributeComponent.h"
#include "ReticleWidget.h"
#include "Blueprint/WidgetTree.h"
#include "AmmoWidget.h"
#include "HighlightInteractable.h"

class UAmmoWidget* UPlayerHUDWidget::GetAmmoWidget()
{
	return WidgetTree->FindWidget<UAmmoWidget>(AmmoWidgetName);
}

UReticleWidget* UPlayerHUDWidget::GetReticleWidget()
{
	return WidgetTree->FindWidget<UReticleWidget>(ReticleWidgetName);
}

void UPlayerHUDWidget::SetHighLightInteractableVisibility(bool bIsVisible)
{
	if (!IsValid(InteractableKey))
	{
		return;
	}
	if (bIsVisible)
	{
		InteractableKey->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InteractableKey->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHUDWidget::SetHighLightInteractableActionText(FName KeyName)
{
	if (IsValid(InteractableKey))
	{
		InteractableKey->SetActionText(KeyName);
	}
}

float UPlayerHUDWidget::GetHealthPercent() const
{
	float Result = 1.0f;
	APawn* Pawn = GetOwningPlayerPawn();
	AXYZBaseCharacter* Character = Cast<AXYZBaseCharacter>(Pawn);
	if (IsValid(Character))
	{
		const UCharacterAttributeComponent* CharacterAttributes = Character->GetCharacterAttributesComponent();
		Result = CharacterAttributes->GetHealthPercent();
	}
	return Result;
}
