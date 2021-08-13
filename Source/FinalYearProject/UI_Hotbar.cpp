// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Hotbar.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"

UUI_Hotbar::UUI_Hotbar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_Selected = 0;

	m_SelectedImagePath = FString("/Game/RetroWindowsGUI/Windows_Inner_Frame_Inverted");
	m_UnselectedImagePath = FString("/Game/RetroWindowsGUI/Windows_Inner_Frame");
}

void UUI_Hotbar::NativeConstruct()
{
	Super::NativeConstruct();

	//Border1->SetBrushColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f));
	UTexture2D* imageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *m_SelectedImagePath));
	Slot1BG->SetBrushFromTexture(imageTexture);

	m_Backgrounds.Add(Slot1BG);
	m_Backgrounds.Add(Slot2BG);
	m_Backgrounds.Add(Slot3BG);
}

void UUI_Hotbar::SetSelected(int index)
{
	if (index != m_Selected)
	{
		UTexture2D* imageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *m_SelectedImagePath));
		m_Backgrounds[index]->SetBrushFromTexture(imageTexture);
		imageTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *m_UnselectedImagePath));
		m_Backgrounds[m_Selected]->SetBrushFromTexture(imageTexture);

		m_Selected = index;
	}
}