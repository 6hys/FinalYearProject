// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ShopInterface.h"
#include "Components/ScrollBox.h"
#include "UObject/ConstructorHelpers.h"
#include "UI_ShopItem.h"
#include "UI_InventoryItem.h"

UUI_ShopInterface::UUI_ShopInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUI_InventoryItem> item(TEXT("/Game/FirstPerson/UI/InventoryItem"));
	if (item.Succeeded())
	{
		m_InventoryItemClass = item.Class;
	}

	static ConstructorHelpers::FClassFinder<UUI_ShopItem> shopItem(TEXT("/Game/FirstPerson/UI/ShopItem"));
	if (shopItem.Succeeded())
	{
		m_ShopItemClass = shopItem.Class;
	}

	// Get the seed data table
	static ConstructorHelpers::FObjectFinder<UDataTable> seedDataTableObject(TEXT("DataTable'/Game/FirstPerson/Data/SeedData.SeedData'"));
	if (seedDataTableObject.Succeeded())
	{
		m_SeedDataTable = seedDataTableObject.Object;
	}
}

void UUI_ShopInterface::SetupSellScreen(TArray<FSeedData> crops, int money)
{
	// Set title texts
	FString title = FString("Selling...");
	ShopTitleText->SetText(FText::FromString(title));

	title = FString("Crops");
	InventoryTitleText->SetText(FText::FromString(title));

	// Clear scroll boxes.
	if (InventoryBox)
	{
		for (UWidget* child : InventoryBox->GetAllChildren())
		{
			if (Cast<UUI_InventoryItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	if (ShopBox)
	{
		for (UWidget* child : ShopBox->GetAllChildren())
		{
			if (Cast<UUI_ShopItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	// Now re-add items.
	for (FSeedData data : crops)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_InventoryItemClass);

		widget->SetNameText(data.RowName);
		widget->SetAmountText(data.Amount);
		widget->SetValueText(data.Value);
		widget->SetImage(data.ImagePath);
		widget->ShowButton();

		InventoryBox->AddChild(widget);
	}

	m_ShopItems.Empty();

	// setup money text 
	FString moneyText = FString("");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}

void UUI_ShopInterface::SetupBuyScreen(TArray<FSeedData> seeds, int money)
{
	// Set title texts
	FString title = FString("The Market");
	ShopTitleText->SetText(FText::FromString(title));

	title = FString("Seeds");
	InventoryTitleText->SetText(FText::FromString(title));

	// Clear scroll boxes.
	if (InventoryBox)
	{
		for (UWidget* child : InventoryBox->GetAllChildren())
		{
			if (Cast<UUI_InventoryItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	if (ShopBox)
	{
		for (UWidget* child : ShopBox->GetAllChildren())
		{
			if (Cast<UUI_ShopItem>(child))
			{
				child->RemoveFromParent();
			}
		}
	}

	// Now re-add items.
	for (FSeedData data : seeds)
	{
		UUI_InventoryItem* widget = CreateWidget<UUI_InventoryItem>(GetWorld(), m_InventoryItemClass);

		widget->SetNameText(data.RowName);
		widget->SetAmountText(data.Amount);
		widget->SetValueText(data.Value);
		widget->SetImage(data.ImagePath);

		InventoryBox->AddChild(widget);
	}

	SetupShopItems();

	for (FSeedData* data : m_ShopItems)
	{
		UUI_ShopItem* widget = CreateWidget<UUI_ShopItem>(GetWorld(), m_ShopItemClass);

		widget->SetNameText(data->RowName);
		widget->SetGrowthTimeText(data->GrowthTime);
		widget->SetCostText(data->Price);
		widget->SetImage(data->ImagePath);
		widget->ShowButton();

		ShopBox->AddChild(widget);
	}

	// setup money text 
	FString moneyText = FString("");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}

void UUI_ShopInterface::UpdateCrop(FString name)
{
	// find the inv item with the crop
	if (InventoryBox)
	{
		for (UWidget* child : InventoryBox->GetAllChildren())
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

void UUI_ShopInterface::UpdateSeed(FString name)
{
	if (InventoryBox)
	{
		for (UWidget* child : InventoryBox->GetAllChildren())
		{
			UUI_InventoryItem* itemChild = Cast<UUI_InventoryItem>(child);

			if (itemChild && itemChild->GetNameText() == name)
			{
				itemChild->IncreaseAmount();
			}
		}
	}
}

void UUI_ShopInterface::RemoveCrop(FString name)
{
	// find the inv item with the crop
	if (InventoryBox)
	{
		for (UWidget* child : InventoryBox->GetAllChildren())
		{
			UUI_InventoryItem* itemChild = Cast<UUI_InventoryItem>(child);

			if (itemChild && itemChild->GetNameText() == name)
			{
				// Item found, remove
				InventoryBox->RemoveChild(itemChild);
				break;
			}
		}
	}
}

void UUI_ShopInterface::AddSeed(FString name)
{
	if (InventoryBox)
	{

		UUI_InventoryItem* newChild = CreateWidget<UUI_InventoryItem>(GetWorld(), m_InventoryItemClass);

		FSeedData* data = m_SeedDataTable->FindRow<FSeedData>(FName(*name), "");

		if (newChild)
		{
			newChild->SetNameText(name);
			newChild->SetAmountText(1);
			newChild->SetGrowthTimeText(data->GrowthTime);
			newChild->SetImage(data->ImagePath);

			InventoryBox->AddChild(newChild);
		}
	}
}

void UUI_ShopInterface::AddSoldItem(FString name)
{
	bool newItem = true;

	if (ShopBox)
	{
		for (UWidget* child : ShopBox->GetAllChildren())
		{
			UUI_ShopItem* itemChild = Cast<UUI_ShopItem>(child);

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
			UUI_ShopItem* newChild = CreateWidget<UUI_ShopItem>(GetWorld(), m_ShopItemClass);

			FSeedData* data = m_SeedDataTable->FindRow<FSeedData>(FName(*name), "");

			if (newChild)
			{
				newChild->SetNameText(name);
				newChild->SetAmountText(1);
				newChild->SetValueText(data->Value);
				newChild->SetImage(data->ImagePath);

				ShopBox->AddChild(newChild);
			}
		}
	}
}

void UUI_ShopInterface::UpdateMoney(int money)
{
	// update money text 
	FString moneyText = FString("");
	moneyText.AppendInt(money);
	Money->SetText(FText::FromString(moneyText));
}

void UUI_ShopInterface::SetupShopItems()
{
	m_ShopItems.Empty();

	TArray<FName> rowNames = m_SeedDataTable->GetRowNames();
	for (FName name : rowNames)
	{
		m_ShopItems.Add(m_SeedDataTable->FindRow<FSeedData>(name, ""));
	}
}