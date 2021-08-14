// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_SellingInterface.h"
#include "Components/ScrollBox.h"
#include "UObject/ConstructorHelpers.h"
#include "UI_SellingItem.h"
#include "UI_InventoryItem.h"

UUI_SellingInterface::UUI_SellingInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUI_InventoryItem> item(TEXT("/Game/FirstPerson/UI/InventoryItem"));
	if (item.Succeeded())
	{
		m_CropItemClass = item.Class;
	}

	static ConstructorHelpers::FClassFinder<UUI_SellingItem> soldItem(TEXT("/Game/FirstPerson/UI/SellingItem"));
	if (soldItem.Succeeded())
	{
		m_SoldItemClass = soldItem.Class;
	}

	// Get the seed data table
	static ConstructorHelpers::FObjectFinder<UDataTable> seedDataTableObject(TEXT("DataTable'/Game/FirstPerson/Data/SeedData.SeedData'"));
	if (seedDataTableObject.Succeeded())
	{
		m_SeedDataTable = seedDataTableObject.Object;
	}
}

void UUI_SellingInterface::Setup(TArray<FSeedData> crops, int money)
{
	// Clear scroll boxes.
	if (CropBox)
	{
		for (UWidget* child : CropBox->GetAllChildren())
		{
			if (Cast<UUI_InventoryItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	if (SellingBox)
	{
		for (UWidget* child : SellingBox->GetAllChildren())
		{
			if (Cast<UUI_SellingItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	// Now re-add items.
	for (FSeedData data : crops)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_CropItemClass);

		widget->SetNameText(data.RowName);
		widget->SetAmountText(data.Amount);
		widget->SetValueText(data.Value);
		widget->SetImage(data.ImagePath);
		widget->ShowButton();

		CropBox->AddChild(widget);
	}

	m_SoldItems.Empty();

	// setup money text 
	FString moneyText = FString("");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}

void UUI_SellingInterface::UpdateCrop(FString name)
{
	// find the inv item with the crop
	if (CropBox)
	{
		for (UWidget* child : CropBox->GetAllChildren())
		{
			UUI_InventoryItem* itemChild = Cast<UUI_InventoryItem>(child);

			if (itemChild && itemChild->GetNameText() == name)
			{
				// Item found, reduce amount
				itemChild->ReduceAmount();
			}
		}
	}
}

void UUI_SellingInterface::RemoveCrop(FString name)
{
	// find the inv item with the crop
	if (CropBox)
	{
		for (UWidget* child : CropBox->GetAllChildren())
		{
			UUI_InventoryItem* itemChild = Cast<UUI_InventoryItem>(child);

			if (itemChild && itemChild->GetNameText() == name)
			{
				// Item found, remove
				CropBox->RemoveChild(itemChild);
			}
		}
	}
}

void UUI_SellingInterface::AddSoldItem(FString name)
{
	bool newItem = true;

	if (SellingBox)
	{
		for (UWidget* child : SellingBox->GetAllChildren())
		{
			UUI_SellingItem* itemChild = Cast<UUI_SellingItem>(child);

			if (itemChild && itemChild->GetNameText() == name)
			{
				newItem = false;

				// Update amount
				itemChild->IncreaseAmount();

				break;
			}
		}

		if (newItem)
		{
			UUI_SellingItem* newChild = CreateWidget<UUI_SellingItem>(GetWorld(), m_SoldItemClass);

			FSeedData* data = m_SeedDataTable->FindRow<FSeedData>(FName(*name), "");

			if (newChild)
			{
				newChild->SetNameText(name);
				newChild->SetAmountText(1);
				newChild->SetValueText(data->Value);
				newChild->SetImage(data->ImagePath);

				SellingBox->AddChild(newChild);
			}
		}
	}
}

void UUI_SellingInterface::UpdateMoney(int money)
{
	// update money text 
	FString moneyText = FString("");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}
