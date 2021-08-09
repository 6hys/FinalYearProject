// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PauseMenu.h"

#include "Kismet/GameplayStatics.h"

#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectHUD.h"
#include "UI_Hotbar.h"

UUI_PauseMenu::UUI_PauseMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup the player controller
	auto world = GetWorld();
	if (world)
	{
		m_Controller = Cast<AFinalYearProjectPlayerController>(world->GetFirstPlayerController());
	}

	bIsFocusable = true;
}

// Quit the game
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

// Close the pause menu
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

		AFinalYearProjectCharacter* character = Cast<AFinalYearProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		// Only show the hotbar if the inventory isnt open
		if (character->IsInventoryOpen() == false)
		{
			// create new hotbar 
			character->MakeNewHotbar();
			character->GetHotbar()->AddToViewport(9999);
			character->GetHotbar()->SetSelected(character->GetCurrentlyEquipped());
		}
	}
}

// Hide the HUD - Open the pause menu or quit the game 
void UUI_PauseMenu::RemoveHUD()
{
	AFinalYearProjectHUD* hud = Cast<AFinalYearProjectHUD>(m_Controller->GetHUD());

	hud->SetPaused(true);
}