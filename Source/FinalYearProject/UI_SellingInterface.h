// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SellingInterface.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_SellingInterface : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_SellingInterface(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* CropBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* SellingBox;
	
};
