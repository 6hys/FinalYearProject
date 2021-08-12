// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

	// Set Name
	FORCEINLINE void SetNameText(FString text) { Name->SetText(FText::FromString(text)); }
	// Get Name
	FORCEINLINE FString GetNameText() { return Name->GetText().ToString(); }

	void SetAmountText(int amt);

	void SetGrowthTimeText(int growth);

	void SetValueText(int val);

	void SetImage(FString path);

	void IncreaseAmount();

private:

	int m_Amount;
	int m_Value;

};
