// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Components/CanvasPanelSlot.h"
#include "UObject/ConstructorHelpers.h"

AFinalYearProjectHUD::AFinalYearProjectHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	// Get the pause menu blueprint
	static ConstructorHelpers::FClassFinder<UUI_PauseMenu> PauseMenu(TEXT("/Game/FirstPerson/UI/PauseMenu"));
	if (PauseMenu.Succeeded())
	{
		m_PauseMenuClass = PauseMenu.Class;
	}

	m_isPaused = false;
}

void AFinalYearProjectHUD::BeginPlay()
{
	Super::BeginPlay();

	// Get the player controller
	m_Controller = Cast<AFinalYearProjectPlayerController>(GetWorld()->GetFirstPlayerController());
}


void AFinalYearProjectHUD::DrawHUD()
{
	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	Super::DrawHUD();

	if (m_isPaused)
	{
		// TEMP SPACE
		// I'll probably have to use this later
	}
	else
	{
		// Draw very simple crosshair

		// find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition((Center.X),
			(Center.Y + 20.0f));

		// draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AFinalYearProjectHUD::SetPaused(bool isPaused)
{
	if (m_isPaused == isPaused) return;

	m_isPaused = isPaused;

	if(isPaused)
	{
		// create the pause menu 
		m_PauseMenu = CreateWidget<UUI_PauseMenu>(GetWorld(), m_PauseMenuClass);

		// Update the input mode
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(m_PauseMenu->GetCachedWidget());
		m_Controller->SetInputMode(inputMode);

		// Display the pause menu
		m_PauseMenu->AddToViewport(9999);
	}
}