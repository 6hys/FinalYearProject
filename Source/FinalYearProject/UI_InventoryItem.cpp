// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InventoryItem.h"
#include "Engine/Texture.h"
#include "Components/Image.h"
#include "Components/Button.h"

UUI_InventoryItem::UUI_InventoryItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUI_InventoryItem::SetAmountText(int amt)
{
	FString amountString = FString("x ");
	amountString.AppendInt(amt);

	m_Amount = amt;

	Amount->SetText(FText::FromString(amountString));
}

void UUI_InventoryItem::SetValueText(int val)
{
	FString valueString = FString("Value: ");
	valueString.AppendInt(val);

	Value->SetText(FText::FromString(valueString));
}

void UUI_InventoryItem::SetGrowthTimeText(int growth)
{
	FString growthString = FString("Growth Time: ");
	growthString.AppendInt(growth);

	Value->SetText(FText::FromString(growthString));
}

void UUI_InventoryItem::SetImage(FString path)
{
	UTexture* imageTexture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *path));

	Icon->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("Image")), imageTexture);
}

void UUI_InventoryItem::ShowButton()
{
	BuySellButton->SetVisibility(ESlateVisibility::Visible);
}

void UUI_InventoryItem::ReduceAmount()
{
	m_Amount--;
	SetAmountText(m_Amount);
}