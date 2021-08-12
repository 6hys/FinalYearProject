// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "SeedData.h"
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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Money;

	void Setup(TArray<FSeedData> crops, int money);

	void UpdateCrop(FString name);

	void RemoveCrop(FString name);

	void AddSoldItem(FString name);

	void UpdateMoney(int money);

private:

	TSubclassOf<class UUI_InventoryItem> m_CropItemClass;
	TSubclassOf<class UUI_SellingItem> m_SoldItemClass;

	TArray<FSeedData> m_SoldItems;

	UDataTable* m_SeedDataTable;

};
