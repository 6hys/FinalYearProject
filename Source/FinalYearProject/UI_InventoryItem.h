// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_InventoryItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUI_InventoryItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Amount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Value;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuySellButton;

	// Set Name
	FORCEINLINE void SetNameText(FString text) { Name->SetText(FText::FromString(text)); }
	// Get Name
	FORCEINLINE FString GetNameText() { return Name->GetText().ToString(); }
	
	void SetAmountText(int amt);
	
	void SetValueText(int val);

	void SetGrowthTimeText(int growth);

	void SetImage(FString path);

	void ShowButton();

	void ReduceAmount();

	void IncreaseAmount();

private:

	int m_Amount;
};
