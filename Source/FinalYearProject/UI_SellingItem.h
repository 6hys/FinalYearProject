// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SellingItem.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_SellingItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_SellingItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Amount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Value;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Icon;

};
