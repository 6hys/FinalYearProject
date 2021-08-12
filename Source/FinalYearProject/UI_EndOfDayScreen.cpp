// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EndOfDayScreen.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "FinalYearProjectHUD.h"

UUI_EndOfDayScreen::UUI_EndOfDayScreen(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup the player controller
	auto world = GetWorld();
	if (world)
	{
		m_Controller = Cast<AFinalYearProjectPlayerController>(world->GetFirstPlayerController());
	}
}

void UUI_EndOfDayScreen::SetTitleText(int dayCount)
{
 	FString text = FString("End Of Day ");
	text.AppendInt(dayCount);

	Title->SetText(FText::FromString(text));
}

void UUI_EndOfDayScreen::SetMoneyText(int money)
{
	FString moneyText = FString("Money: ");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}

void UUI_EndOfDayScreen::Quit()
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

// Hide the HUD - Open the pause menu or quit the game 
void UUI_EndOfDayScreen::RemoveHUD()
{
	AFinalYearProjectHUD* hud = Cast<AFinalYearProjectHUD>(m_Controller->GetHUD());

	hud->SetPaused(true);
}