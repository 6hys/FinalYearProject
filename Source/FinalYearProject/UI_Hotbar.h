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
	class UImage* Slot1BG;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot2BG;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot3BG;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Slot3;

	void SetSelected(int index);

private:

	virtual void NativeConstruct() override;

	TArray<class UImage*> m_Backgrounds;
	int m_Selected;

	FString m_SelectedImagePath;
	FString m_UnselectedImagePath;
	
};
