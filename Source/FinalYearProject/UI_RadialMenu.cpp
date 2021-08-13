// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RadialMenu.h"

#include "Engine.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Color.h"

#include "UI_SeedItem.h"

UUI_RadialMenu::UUI_RadialMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initialised to closed
	m_IsOpen = false;

	// Initial vector positions
	m_WidgetCenterPoint = FVector2D(256.0f);
	m_WidgetSize = FVector2D(512.0f);
	m_ItemSize = FVector2D(175.0f);

	// Get the seed data table
	static ConstructorHelpers::FObjectFinder<UDataTable> seedDataTableObject(TEXT("DataTable'/Game/FirstPerson/Data/SeedData.SeedData'"));
	if (seedDataTableObject.Succeeded())
	{
		m_SeedDataTable = seedDataTableObject.Object;
	}

	static ConstructorHelpers::FClassFinder<UUI_SeedItem> seedItem(TEXT("/Game/FirstPerson/UI/RadialMenu/Item"));
	if (seedItem.Succeeded())
	{
		m_ItemClass = seedItem.Class;
	}
}

void UUI_RadialMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Get the cursor image
	if (IsValid(CursorImg))
	{
		m_DynamicMaterial = CursorImg->GetDynamicMaterial();
	}
}

void UUI_RadialMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Find full screen resolution
	TArray<FIntPoint> resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions);
	m_ViewportSize = resolutions[resolutions.Num() - 1];

	// TODO: Need to set up inventory before doing this
	//m_Seeds = Inventory->GetSeedInventory();
	// TEMP INIT ALL
	// Gets the full list of seeds
	SetupSeedItems();

	// Creates all the items
	CreateItems();
}

void UUI_RadialMenu::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (m_IsOpen)
	{
		// Maths to work out the direction of the mouse from the center of the screen.
		// https://www.youtube.com/watch?v=OHEk3MkzNv0
		FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		
		const FVector2D centerPosition = FVector2D(m_ViewportSize.X / 2.0f, m_ViewportSize.Y / 2.0f);
	
		float direction = -1.0f;
		if (mousePosition.X - centerPosition.X >= 0.0f)
		{
			direction = 1.0f;
		}

		FVector2D currentDirection = FVector2D(mousePosition - centerPosition);
		currentDirection.Normalize();

		FVector2D topCenter = FVector2D(centerPosition.X, 0.0f);

		FVector2D baseDirection = FVector2D(topCenter - centerPosition);
		baseDirection.Normalize();

		float dotProduct = FVector2D::DotProduct(currentDirection, baseDirection);

		m_Rotation = UKismetMathLibrary::DegAcos(dotProduct) * direction;

		CursorImg->SetRenderTransformAngle(m_Rotation);

		ItemName->SetRenderOpacity(m_Opacity);

		if (m_DynamicMaterial)
		{
			m_DynamicMaterial->SetScalarParameterValue(FName(TEXT("Opacity")), m_Opacity);
		}

		int currentIndex = FindCurrentItem();
		// TEMP
		//int currentIndex = 0;

		if (m_OldIndex != currentIndex)
		{
			// Deselect the old item
			if (IsValid(m_SeedItems[m_OldIndex]))
			{
				m_SeedItems[m_OldIndex]->ItemAmount->SetColorAndOpacity(FSlateColor(FLinearColor(0.4f, 0.4f, 0.3f, 1.0f)));
			}

			// Highlight the selected item
			FLinearColor highlightColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

			if (IsValid(m_SeedItems[currentIndex]))
			{
				m_SeedItems[currentIndex]->ItemAmount->SetColorAndOpacity(FSlateColor(highlightColor));

				// Update the current item
				m_CurrentItem = m_SeedItems[currentIndex];

				// Update the text box
				ItemName->SetText(FText::FromString(m_CurrentItem->GetName()));
			}

			// Update the old index
			m_OldIndex = currentIndex;
		}
	}
}

FVector2D UUI_RadialMenu::GetWidgetPosition(int index)
{
	float angle = 360.0f / m_Seeds.Num();

	angle *= index;
	angle -= 90.0f;

	FVector2D position = FVector2D((m_WidgetSize.X / 1.75f), 0.0f);
	position = position.GetRotated(angle);
	position += m_WidgetCenterPoint;

	FVector2D size = m_ItemSize / 2.0f;

	position -= size;

	return position;
}

int UUI_RadialMenu::FindCurrentItem()
{
	// Convert to 0-360 range
	float rot = m_Rotation;
	
	if (m_Rotation < 0.0f)
	{
		rot = 360.0f - FMath::Abs(m_Rotation);
	}

	float div = 180.0f / m_Seeds.Num();

	int floor = FMath::FloorToFloat((rot - div) / (div * 2.0f));
	floor += 1;

	int item = 0;

	if (m_Seeds.Num() == floor)
	{
		item = m_Seeds.Num();
	}

	return (floor - item);
}

void UUI_RadialMenu::CreateItems()
{
	// Clear widgets.
	if (m_SeedItems.Num() > 0)
	{
		for (int i = 0; i < m_SeedItems.Num(); i++)
		{
			Canvas->RemoveChild(m_SeedItems[i]);
		}

		// Clear the array
		m_SeedItems.Empty();
	}

	// Loop over the array of items
	for (int i = 0; i < m_Seeds.Num(); i++)
	{
		if (m_Seeds.IsValidIndex(i))
		{
			FSeedData* seed = m_Seeds[i];

			UUI_SeedItem* widget = CreateWidget<UUI_SeedItem>(GetWorld(), m_ItemClass);

			widget->init(*seed);

			m_SeedItems.Add(widget);

			Canvas->AddChildToCanvas(widget);

			UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(widget->Slot);
			FAnchors widgetAnchors = FAnchors();
			widgetSlot->SetAnchors(widgetAnchors);
			widgetSlot->SetSize(m_ItemSize);
			widgetSlot->SetPosition(GetWidgetPosition(i));

			widget->SetRenderOpacity(1.0f);
		}
	}
}

void UUI_RadialMenu::SetupSeedItems()
{
	m_Seeds.Empty();

	TArray<FName> rowNames = m_SeedDataTable->GetRowNames();
	for (FName name : rowNames)
	{
		m_Seeds.Add(m_SeedDataTable->FindRow<FSeedData>(name, ""));
	}
}

FText UUI_RadialMenu::GetText()
{
	if (IsValid(m_CurrentItem))
	{
		return FText::FromString(m_CurrentItem->GetName());
	}
	else
	{
		return FText();
	}
}

void UUI_RadialMenu::SetWidgetSize(FVector2D size)
{
	m_WidgetSize = size;
}

void UUI_RadialMenu::SetWidgetCenterPoint(FVector2D point)
{
	m_WidgetCenterPoint = point;
}

bool UUI_RadialMenu::GetIsOpen()
{
	return m_IsOpen;
}

void UUI_RadialMenu::SetIsOpen(bool open)
{
	UE_LOG(LogTemp, Display, TEXT("Setting open"));
	m_IsOpen = open;
}

void UUI_RadialMenu::SetOpacity(float op)
{
	UE_LOG(LogTemp, Display, TEXT("Setting opacity to %f"), op);
	m_Opacity = op;
}

TArray<UUI_SeedItem*> UUI_RadialMenu::GetSeedItems()
{
	return m_SeedItems;
}

UUI_SeedItem* UUI_RadialMenu::GetCurrentItem()
{
	return m_CurrentItem;
}