// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FinalYearProjectSaveGame.h"
#include "UI_LoadGameItem.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_LoadGameItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_LoadGameItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SlotName;

	// Set Slot Name
	FORCEINLINE void SetSlotName(FString name) { SlotName->SetText(FText::FromString(name)); }
};
