// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "UI_InventoryItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ScrollBox.h"

UUI_Inventory::UUI_Inventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUI_InventoryItem> item(TEXT("/Game/FirstPerson/UI/InventoryItem"));
	if (item.Succeeded())
	{
		m_ItemClass = item.Class;
	}
}

void UUI_Inventory::AddToInventory(FSeedData data, ItemType type)
{
	// Check if items already in inventory - Add to amount instead
	// Else add to the array.
	int i = 0;

	switch (type) {
		case Seed:
			i = m_SeedItems.Find(data);
			if (i != INDEX_NONE)
			{
				m_SeedItems[i].Amount++;
			}
			else
			{
				data.Amount = 1;
				m_SeedItems.Add(data);
			}
			break;
		case Crop:
			i = m_CropItems.Find(data);
			if (i != INDEX_NONE)
			{
				m_CropItems[i].Amount++;
			}
			else
			{
				data.Amount = 1;
				m_CropItems.Add(data);
			}
			break;
		default:
			break;
	}

	Refresh();
}

void UUI_Inventory::Refresh()
{
	// Clear scroll boxes.
	for (UWidget* child : CropsBox->GetAllChildren())
	{
		if (Cast<UUI_InventoryItem>(child))
		{
			child->RemoveFromParent();
		}
	}
	for (UWidget* child : SeedsBox->GetAllChildren())
	{
		if (Cast<UUI_InventoryItem>(child))
		{
			child->RemoveFromParent();
		}
	}

	// Now re-add items.
	for (FSeedData data : m_CropItems)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_ItemClass);

		widget->SetNameText(data.RowName);
		widget->SetAmountText(data.Amount);
		widget->SetValueText(data.Value);
		widget->SetImage(data.ImagePath);

		CropsBox->AddChild(widget);
	}
	for (FSeedData data : m_SeedItems)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_ItemClass);

		widget->SetNameText(data.SeedName);
		widget->SetAmountText(data.Amount);
		widget->SetGrowthTimeText(data.GrowthTime);
		widget->SetImage(data.ImagePath);

		SeedsBox->AddChild(widget);
	}
}