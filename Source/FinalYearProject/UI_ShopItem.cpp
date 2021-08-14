// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ShopItem.h"
#include "Engine/Texture.h"
#include "Components/Image.h"
#include "Components/Button.h"

UUI_ShopItem::UUI_ShopItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUI_ShopItem::SetAmountText(int amt)
{
	FString amountString = FString("x");
	amountString.AppendInt(amt);

	m_Amount = amt;

	AmountTime->SetText(FText::FromString(amountString));
}

void UUI_ShopItem::SetGrowthTimeText(int growth)
{
	FString growthString = FString("Growth Time: ");
	growthString.AppendInt(growth);

	AmountTime->SetText(FText::FromString(growthString));
}

void UUI_ShopItem::SetValueText(int val)
{
	FString valueString = FString("Value: ");
	valueString.AppendInt(val);

	m_Value = val / m_Amount;

	ValueCost->SetText(FText::FromString(valueString));
}

void UUI_ShopItem::SetCostText(int cost)
{
	FString costString = FString("Price: ");
	costString.AppendInt(cost);
	ValueCost->SetText(FText::FromString(costString));
}

void UUI_ShopItem::SetImage(FString path)
{
	UTexture* imageTexture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *path));

	Icon->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("Image")), imageTexture);
}

void UUI_ShopItem::IncreaseAmount()
{
	m_Amount++;
	SetAmountText(m_Amount);
	SetValueText(m_Amount * m_Value);
}

void UUI_ShopItem::ShowButton()
{
	BuyButton->SetVisibility(ESlateVisibility::Visible);
}