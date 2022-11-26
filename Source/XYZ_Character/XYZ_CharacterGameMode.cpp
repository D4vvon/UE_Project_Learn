// Copyright Epic Games, Inc. All Rights Reserved.

#include "XYZ_CharacterGameMode.h"
#include "XYZ_CharacterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AXYZ_CharacterGameMode::AXYZ_CharacterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
