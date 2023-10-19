// Copyright Epic Games, Inc. All Rights Reserved.

#include "d3d_ue5GameMode.h"
#include "d3d_ue5Character.h"
#include "UObject/ConstructorHelpers.h"

Ad3d_ue5GameMode::Ad3d_ue5GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
