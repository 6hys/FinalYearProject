// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "FinalYearProjectPlayerController.h"
#include "UI_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_PauseMenu(const FObjectInitializer& ObjectInitializer);

private:

	AFinalYearProjectPlayerController* m_Controller;

	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void Quit();

	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void Resume();

	void RemoveHUD();

};
