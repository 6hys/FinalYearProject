// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "UI_InventoryItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

UUI_Inventory::UUI_Inventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUI_InventoryItem> item(TEXT("/Game/FirstPerson/UI/InventoryItem"));
	if (item.Succeeded())
	{
		m_ItemClass = item.Class;
	}
}

void UUI_Inventory::Setup(TArray<FSeedData> seeds, TArray<FSeedData> crops, int money)
{
	// Clear scroll boxes.
	if (CropsBox)
	{
		for (UWidget* child : CropsBox->GetAllChildren())
		{
			if (Cast<UUI_InventoryItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	if (SeedsBox)
	{
		for (UWidget* child : SeedsBox->GetAllChildren())
		{
			if (Cast<UUI_InventoryItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	// Now re-add items.
	for (FSeedData data : crops)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_ItemClass);

		widget->SetNameText(data.RowName);
		widget->SetAmountText(data.Amount);
		widget->SetValueText(data.Value);
		widget->SetImage(data.ImagePath);

		CropsBox->AddChild(widget);
	}
	for (FSeedData data : seeds)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_ItemClass);

		widget->SetNameText(data.SeedName);
		widget->SetAmountText(data.Amount);
		widget->SetGrowthTimeText(data.GrowthTime);
		widget->SetImage(data.ImagePath);

		SeedsBox->AddChild(widget);
	}

	// setup money text 
	FString moneyText = FString("");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}