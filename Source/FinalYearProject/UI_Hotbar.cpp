// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Hotbar.h"

#include "Components/Border.h"

UUI_Hotbar::UUI_Hotbar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_Selected = 0;
}

void UUI_Hotbar::NativeConstruct()
{
	Super::NativeConstruct();

	Border1->SetBrushColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f));

	m_Borders.Add(Border1);
	m_Borders.Add(Border2);
	m_Borders.Add(Border3);
}

void UUI_Hotbar::SetSelected(int index)
{
	if (index != m_Selected)
	{
		m_Borders[index]->SetBrushColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f));
		m_Borders[m_Selected]->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

		m_Selected = index;
	}
}