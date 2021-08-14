// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_ShopItem.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_ShopItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_ShopItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmountTime;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ValueCost;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BuyButton;

	// Set Name
	FORCEINLINE void SetNameText(FString text) { Name->SetText(FText::FromString(text)); }
	// Get Name
	FORCEINLINE FString GetNameText() { return Name->GetText().ToString(); }

	void SetAmountText(int amt);

	void SetGrowthTimeText(int growth);

	void SetValueText(int val);

	void SetCostText(int cost);

	void SetImage(FString path);

	void IncreaseAmount();

	void ShowButton();

private:

	int m_Amount;
	int m_Value;

};
