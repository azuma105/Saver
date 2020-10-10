// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Saver.h"
#include "SaverGameMode.h"
#include "Character/SaverCharacter.h"
#include "Controller/SaverPlayerController.h"

ASaverGameMode::ASaverGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ASaverPlayerController::StaticClass();
}
