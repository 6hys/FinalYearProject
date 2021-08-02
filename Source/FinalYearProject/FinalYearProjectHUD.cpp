// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AFinalYearProjectHUD::AFinalYearProjectHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

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

	if (m_PauseMenuClass)
	{
		m_PauseMenu = CreateWidget<UUI_PauseMenu>(GetWorld(), m_PauseMenuClass);
		m_PauseMenu->RemoveFromViewport();
	}
}


void AFinalYearProjectHUD::DrawHUD()
{
	Super::DrawHUD();

	if (m_isPaused)
	{
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
	m_isPaused = isPaused;

	if(isPaused)
	{
		AFinalYearProjectPlayerController* controller = Cast<AFinalYearProjectPlayerController>(GetWorld()->GetFirstPlayerController());

		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(m_PauseMenu->GetCachedWidget());
		controller->SetInputMode(inputMode);

		m_PauseMenu->AddToViewport(9999);
	}
}