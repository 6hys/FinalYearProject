// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SeedData.h"
#include "UI_Inventory.generated.h"

enum ItemType
{
	Seed,
	Crop
};

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

	void AddToInventory(FSeedData data, ItemType type);

	void Refresh();

private:
	// Arrays to store the information about the items
	TArray<FSeedData> m_SeedItems;
	TArray<FSeedData> m_CropItems;
	
	TSubclassOf<class UUI_InventoryItem> m_ItemClass;

public:
	// Get seed inventory
	FORCEINLINE TArray<FSeedData> GetSeedInventory() { return m_SeedItems; }
	// Get crop inventory
	FORCEINLINE TArray<FSeedData> GetCropInventory() { return m_CropItems; }
	// Set seed inventory 
	FORCEINLINE void SetSeedInventory(TArray<FSeedData> seeds) { m_SeedItems = seeds; }
	// Set crop inventory
	FORCEINLINE void SetCropInventory(TArray<FSeedData> crops) { m_CropItems = crops; }
};
