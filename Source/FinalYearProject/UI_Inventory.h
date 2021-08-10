// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SeedData.h"
#include "UI_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_Inventory(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* CropsBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* SeedsBox;

	void Setup(TArray<FSeedData> seeds, TArray<FSeedData> crops);

private:

	TSubclassOf<class UUI_InventoryItem> m_ItemClass;

};
