// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SellingItem.h"
#include "Engine/Texture.h"
#include "Components/Image.h"

UUI_SellingItem::UUI_SellingItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUI_SellingItem::SetAmountText(int amt)
{
	FString amountString = FString("x ");
	amountString.AppendInt(amt);

	m_Amount = amt;

	Amount->SetText(FText::FromString(amountString));
}

void UUI_SellingItem::SetGrowthTimeText(int growth)
{
	FString growthString = FString("Growth Time: ");
	growthString.AppendInt(growth);

	Amount->SetText(FText::FromString(growthString));
}

void UUI_SellingItem::SetValueText(int val)
{
	FString valueString = FString("Value: ");
	valueString.AppendInt(val);

	m_Value = val;

	Value->SetText(FText::FromString(valueString));
}

void UUI_SellingItem::SetImage(FString path)
{
	UTexture* imageTexture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *path));

	Icon->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("Image")), imageTexture);
}

void UUI_SellingItem::IncreaseAmount()
{
	m_Amount++;
	SetAmountText(m_Amount);
	SetValueText(m_Amount * m_Value);
}