// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectGameMode.h"
#include "FinalYearProjectHUD.h"
#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AFinalYearProjectGameMode::AFinalYearProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFinalYearProjectHUD::StaticClass();

	// player controller
	PlayerControllerClass = AFinalYearProjectPlayerController::StaticClass();
}

/* Code from https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/UMG/ */
void AFinalYearProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AFinalYearProjectGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
