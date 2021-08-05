// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI_PauseMenu.h"
#include "UI_RadialMenu.h"
#include "FinalYearProjectHUD.generated.h"

UCLASS()
class AFinalYearProjectHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFinalYearProjectHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	// Pausing the game
	void SetPaused(bool isPaused);

	TSubclassOf<UUI_PauseMenu> m_PauseMenuClass;

	UUI_PauseMenu* m_PauseMenu;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	bool m_isPaused;
	bool m_openRadial;
};

