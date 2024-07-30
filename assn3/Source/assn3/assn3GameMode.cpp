// Copyright Epic Games, Inc. All Rights Reserved.

#include "assn3GameMode.h"
#include "assn3PlayerController.h"
#include "assn3Character.h"
#include "UObject/ConstructorHelpers.h"

Aassn3GameMode::Aassn3GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = Aassn3PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}