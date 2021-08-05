// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UI_SeedItem.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_SeedItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_SeedItem(const FObjectInitializer& ObjectInitializer);

	void init(struct FSeedData seed);

	// From the blueprint
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemAmount = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage = nullptr;

	FString GetName();

private:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	// Amount of seeds in inventory
	int m_Amount;

	// Image of icon
	UTexture* m_ImageTexture;

	// Name of item
	FString m_Name;

//	UFUNCTION(BlueprintCallable, Category = "My Functions")
};
