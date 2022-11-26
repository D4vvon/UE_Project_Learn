// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MovementComponents/XYZBaseCharacterMovementComp.h"
#include "Curves/CurveVector.h"
#include "Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "XYZTypes.h"
#include "GenericTeamAgentInterface.h"
#include "Actors/Interactive/Interface/Interactable.h"
#include "UObject/ScriptInterface.h"
#include "Engine/EngineTypes.h"
#include "Components/CharacterComponents/CharacterInventoryComponent.h"
#include "XYZBaseCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnInteractableObjectFound, FName);

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionXY = 65.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float AnimationCorrectionZ = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeight = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeight = 125.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxHeightStartTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MinHeightStartTime = 0.5f;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimingStateChanged, bool)

class AInteractiveActor;
class Interactive;
class UXYZBaseCharacterMovementComp;
class UCharacterEquipmentComponent;
class UCharacterAttributeComponent;
UCLASS(Abstract, NotBlueprintable)
class XYZ_CHARACTER_API AXYZBaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AXYZBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void LookUpAtRate(float Value) {};

	virtual bool IsOutOfStamina();

	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();

	virtual void Tick(float DeltaTime) override;

	virtual void SwimForward(float Value) { };
	virtual void SwimRight(float Value) { };
	virtual void SwimUp(float Value) { };

	UFUNCTION(BlueprintCallable)
	virtual void Mantle();

	virtual bool CanJumpInternal_Implementation() const override;

	FORCEINLINE UXYZBaseCharacterMovementComp* GetBaseCharacterMovementComponent() const { return XYZBaseCharacterMovementComponent; }

	UCharacterEquipmentComponent* GetCharacterEquipmentComponent_Mutable();
	const UCharacterEquipmentComponent* GetCharacterEquipmentComponent() const;
	const UCharacterAttributeComponent* GetCharacterAttributesComponent() const;

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);
	const class ALadder* GetAvalibleLadder() const;

	void ClimbLadderUp(float Value);

	void InteractiveWithLadder();

	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void NotifyJumpApex() override;

	void StartFire();
	void StopFire();

	void StartAiming();
	void StopAiming();

	void NextItem();
	void PreviousItem();
	
	void Reload();

	void EquipPrimaryItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStartAiming();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OnStopAiming();

	float GetAimingMovementSpeed() const;

	bool IsAiming() const;
	FOnAimingStateChanged OnAimingStateChanged;

	/* IGenericTeamAgentInterface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* ~IGenericTeamAgentInterface */

	void Interact();

	void AddEquipmentItem(const TSubclassOf<class AEquipableItem> EquipableItemClass);

	bool PickupItem(TWeakObjectPtr<UInventoryItem> ItemToPickup);

	void UseInventory(APlayerController* PlayerController);

	FOnInteractableObjectFound OnInteractableObjectFound;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void OnSprintStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Movement")
	void OnSprintEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Controls")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stamina")
	float StaminaRestoreVelocity = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stamina")
	float SprintStaminaConsuptionVelocity = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	bool bIsFalling = false;


	virtual bool CanSprint();

	UXYZBaseCharacterMovementComp* XYZBaseCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Caracter | Movement| Mantling")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Caracter | Movement| Mantling")
	FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Caracter | Movement| Mantling", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float LowMantleMaxHeight = 125.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	UCharacterAttributeComponent* CharacterAttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	UCharacterInventoryComponent* CharacterInventoryComponent;

	virtual void OnDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Animations")
	class UAnimMontage* OnDeathAnimMontage;

	//Curve damage depending from fall height(in meters)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Attributes")
	class UCurveFloat* FallDamageCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	UCharacterEquipmentComponent* CharacterEquipmentComponent;

	virtual void OnStartAimingInternal();
	virtual void OnStopAimingInternal();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Team")
	ETeams Team = ETeams::Enemy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Trace")
	float LineOfSightDistance = 500.0f;

	void TraceLineOfSight();

	UPROPERTY()
	TScriptInterface<IInteractable> LineOfSightObject;

private:

	void TryChangeSprintStart(float DeltaTime);

	bool bIsSprintRequested = false;

	float CurrentStamina;

	bool bIsAiming;
	float CurrentAimingMovementSpeed;

	const FMantlingSettings& GetMantlingSettings(float LedgeHeight);

	TArray<AInteractiveActor*> AvalibleInteractiveActors;

	void EnableRagdoll();

	FVector CurrentFallApex;
};
