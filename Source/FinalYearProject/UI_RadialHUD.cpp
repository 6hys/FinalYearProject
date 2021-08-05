// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RadialHUD.h"

#include "Components/CanvasPanelSlot.h"

#include "UI_RadialMenu.h"

UUI_RadialHUD::UUI_RadialHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUI_RadialHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UCanvasPanelSlot* radialSlot = Cast<UCanvasPanelSlot>(RadialMenu->Slot);

	FVector2D radialSlotSize = radialSlot->GetSize();

	RadialMenu->SetWidgetSize(radialSlotSize);

	RadialMenu->SetWidgetCenterPoint(radialSlotSize / 2.0f);
	RadialMenu->SetupSeedItems();
	RadialMenu->CreateItems();
}