// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_RadialHUD.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_RadialHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_RadialHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* BGImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUI_RadialMenu* RadialMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim))
	class UWidgetAnimation* InOut;

private:

	virtual void NativeConstruct() override;
};
