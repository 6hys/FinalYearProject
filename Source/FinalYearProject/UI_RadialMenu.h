// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "SeedData.h"
#include "UI_RadialMenu.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UUI_RadialMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UUI_RadialMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* CursorImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Canvas;

	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void CreateItems();

	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void SetWidgetSize(FVector2D size);

	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void SetWidgetCenterPoint(FVector2D point);

	bool GetIsOpen();
	void SetIsOpen(bool open);

	void SetOpacity(float op);

	TArray<class UUI_SeedItem*> GetSeedItems();
	// TEMP?
	void SetupSeedItems();

private:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	bool m_IsOpen;

	float m_Opacity;
	float m_Rotation;

	int m_OldIndex;

	FVector2D m_ItemSize;
	FVector2D m_WidgetSize;
	FVector2D m_WidgetCenterPoint;

	FIntPoint m_ViewportSize;

	TSubclassOf<class UUI_SeedItem> m_ItemClass;
	TArray<FSeedData*> m_Seeds;
	TArray<class UUI_SeedItem*> m_SeedItems;
	UUI_SeedItem* m_CurrentItem;
	UDataTable* m_SeedDataTable;

	class UMaterialInstanceDynamic* m_DynamicMaterial;

	FVector2D GetWidgetPosition(int index);

	int FindCurrentItem();

	FText GetText();
};
