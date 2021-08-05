// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SeedItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "SeedData.h"

UUI_SeedItem::UUI_SeedItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_Amount = 0;
}

void UUI_SeedItem::NativePreConstruct()
{
	Super::NativePreConstruct();

	FString amountString = FString(TEXT("("));

	amountString.AppendInt(m_Amount);
	amountString.Append(TEXT(")"));

	if (ItemAmount)
	{
		ItemAmount->SetText(FText::FromString(amountString));
	}
}

void UUI_SeedItem::NativeConstruct()
{
	Super::NativeConstruct();

	if(!ItemAmount)
		ItemAmount = (UTextBlock*)GetWidgetFromName(TEXT("ItemAmount"));
	if(!ItemImage)
		ItemImage = (UImage*)GetWidgetFromName(TEXT("ItemImage"));

}

void UUI_SeedItem::init(FSeedData seed)
{
	m_Name = seed.Name;
	m_Amount = seed.Amount;

	m_ImageTexture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *(seed.ImagePath)));

	if (ItemImage)
	{
	//	ItemImage->SetBrushFromTexture(m_ImageTexture, false);
	//	ItemImage->Brush.DrawAs = ESlateBrushDrawType::Image;
	//	ItemImage->Brush.ImageType = ESlateBrushImageType::FullColor;
		ItemImage->GetDynamicMaterial()->SetTextureParameterValue(FName(TEXT("Image")), m_ImageTexture);
	}
}

FString UUI_SeedItem::GetName()
{
	return m_Name;
}