// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PauseMenu.h"

#include "Kismet/GameplayStatics.h"

#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectHUD.h"

UUI_PauseMenu::UUI_PauseMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto world = GetWorld();
	if (world)
	{
		m_Controller = Cast<AFinalYearProjectPlayerController>(world->GetFirstPlayerController());
	}

	bIsFocusable = true;
}

void UUI_PauseMenu::Quit()
{
	if (m_Controller)
	{
		m_Controller->bShowMouseCursor = true;

		RemoveHUD();

		RemoveFromParent();

		m_Controller->SetPause(false);

		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

void UUI_PauseMenu::Resume()
{
	if (m_Controller)
	{
		m_Controller->SetInputMode(FInputModeGameOnly());

		m_Controller->bShowMouseCursor = false;

		RemoveFromParent();

		m_Controller->SetPause(false);

		AFinalYearProjectHUD* hud = Cast<AFinalYearProjectHUD>(m_Controller->GetHUD());

		hud->SetPaused(false);
	}
}

void UUI_PauseMenu::RemoveHUD()
{
	AFinalYearProjectHUD* hud = Cast<AFinalYearProjectHUD>(m_Controller->GetHUD());

	hud->SetPaused(true);
}