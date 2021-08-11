// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinalYearProjectPlayerController.h"
#include "UI_EndOfDayScreen.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_EndOfDayScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_EndOfDayScreen(const FObjectInitializer& ObjectInitializer);

	// Set Title text: "End Of Day X"
	void SetTitleText(int dayCount);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SaveQuitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Title;

private:

	UFUNCTION(BlueprintCallable, Category = "My Functions")
		void Quit();

	void RemoveHUD();

	AFinalYearProjectPlayerController* m_Controller;
};
