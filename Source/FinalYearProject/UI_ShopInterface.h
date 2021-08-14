// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "SeedData.h"
#include "UI_ShopInterface.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_ShopInterface : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_ShopInterface(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* InventoryBox; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* ShopBox; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Money;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ShopTitleText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* InventoryTitleText;

	void SetupSellScreen(TArray<FSeedData> crops, int money);

	void SetupBuyScreen(TArray<FSeedData> seeds, int money);

	void UpdateCrop(FString name);

	void UpdateSeed(FString name);

	void RemoveCrop(FString name);

	void AddSeed(FString name);

	void AddSoldItem(FString name);

	void UpdateMoney(int money);

	void SetupShopItems();
	
private:

	TSubclassOf<class UUI_InventoryItem> m_InventoryItemClass; 
	TSubclassOf<class UUI_ShopItem> m_ShopItemClass; 

	TArray<FSeedData*> m_ShopItems;

	UDataTable* m_SeedDataTable;

};
