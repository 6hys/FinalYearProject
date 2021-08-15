// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectGameMode.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

#include "FinalYearProjectHUD.h"
#include "FinalYearProjectPlayerController.h"

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

	static ConstructorHelpers::FClassFinder<UUserWidget> mainMenu(TEXT("/Game/FirstPerson/UI/MainMenu"));
	if (mainMenu.Succeeded())
	{
		m_StartingWidgetClass = mainMenu.Class;
	}
}

void AFinalYearProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
	// controller
	m_Controller = Cast<AFinalYearProjectPlayerController>(GetWorld()->GetFirstPlayerController());

	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (LevelName == FString("MainMenu"))
	{
		m_Controller->SetInputMode(FInputModeUIOnly());
		ChangeMenuWidget(m_StartingWidgetClass);
	}
}

/* Code from https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/UMG/ */
void AFinalYearProjectGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (m_CurrentWidget != nullptr)
	{
		m_CurrentWidget->RemoveFromViewport();
		m_CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		m_CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (m_CurrentWidget != nullptr)
		{
			m_CurrentWidget->AddToViewport();
		}
	}
}