// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Hotbar.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_Hotbar : public UUserWidget
{
	GENERATED_BODY()

public:

	UUI_Hotbar(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot3;

	void SetSelected(int index);

private:

	virtual void NativeConstruct() override;

	TArray<class UBorder*> m_Borders;
	int m_Selected;
	
};
