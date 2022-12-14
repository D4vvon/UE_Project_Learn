// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleWidget.h"
#include "AmmoWidget.h"
#include "HighlightInteractable.h"
#include "PlayerHUDWidget.generated.h"

class UHighlightInteractable;
UCLASS()
class XYZ_CHARACTER_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UAmmoWidget* GetAmmoWidget();

	class UReticleWidget* GetReticleWidget();

	void SetHighLightInteractableVisibility(bool bIsVisible);
	void SetHighLightInteractableActionText(FName KeyName);

protected:

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName ReticleWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName AmmoWidgetName;
	
	UPROPERTY(meta = (BindWidget));
	UHighlightInteractable* InteractableKey;
};
