// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI_Inventory.h"
#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectGameInstance.h"

// Sets default values
AGridBase::AGridBase()
{
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	m_Highlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Highlight"));
	RootComponent = m_Box;

	m_Box->CreationMethod = EComponentCreationMethod::Native;
	m_Box->SetBoxExtent(FVector(100.0f, 100.0f, 10.0f));
	m_Box->SetCollisionProfileName(FName(TEXT("Custom")));
	m_Box->SetCollisionObjectType(ECC_GameTraceChannel2);

	// Stores the dirt texture
	m_Tile->SetupAttachment(m_Box);
	m_Tile->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_Tile->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	m_Tile->SetCollisionProfileName(FName(TEXT("NoCollision")));
	m_Tile->CastShadow = false;

	// TODO: Visible only when looking at the tile. Will show information like growth%,  plant name, ect.
	m_Highlight->SetupAttachment(m_Tile);
	m_Highlight->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	m_Highlight->SetCollisionProfileName(FName(TEXT("NoCollision")));
	m_Highlight->SetVisibility(false);
	m_Highlight->CastShadow = false;
		
	// https://answers.unrealengine.com/questions/40158/how-can-i-access-project-materials-from-code.html
	static ConstructorHelpers::FObjectFinder<UStaticMesh> highlight(TEXT("/Game/Geometry/Meshes/GridHighlight.GridHighlight"));
	if (highlight.Succeeded())
	{
		m_Highlight->SetStaticMesh(highlight.Object);
	}

	// Meshes for dirt tiles
	static ConstructorHelpers::FObjectFinder<UStaticMesh> planted(TEXT("/Game/LowPolyFarm/Meshes/Landscape/Mesh_Landscape_Mud_02.Mesh_Landscape_Mud_02"));
	if (planted.Succeeded())
	{
		m_PlantedMesh = planted.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> unplanted(TEXT("/Game/LowPolyFarm/Meshes/Landscape/Mesh_Landscape_Mud_05.Mesh_Landscape_Mud_05"));
	if (unplanted.Succeeded())
	{
		m_UnplantedMesh = unplanted.Object;
	}

	// get game instance
	m_GameInstance = Cast<UFinalYearProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	// Initialise tile as empty.
	m_State = State::Empty;

	// Set mesh as unplanted
	m_Tile->SetStaticMesh(m_UnplantedMesh);

	// Get material
	m_GridMaterial = UMaterialInstanceDynamic::Create(m_Tile->GetMaterial(0), NULL);
	m_Tile->SetMaterial(0, m_GridMaterial);

	// Create plant mesh component
	m_Plant = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("m_Plant"));
	m_Plant->bCastDynamicShadow = false;
	m_Plant->SetupAttachment(m_Tile, TEXT("Plant"));
	m_Plant->SetupAttachment(RootComponent);
	m_Plant->SetActive(false);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGridBase::OnConstruction(const FTransform& Transform)
{
	//if (m_HighlightMaterial)
	//{
		//m_GridMaterialRef = m_Highlight->CreateDynamicMaterialInstance(0, m_HighlightMaterial);
	//}
}

// Called when the game starts or when spawned
void AGridBase::BeginPlay()
{
	Super::BeginPlay();

	m_Plant->AttachToComponent(m_Tile, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Plant"));
}

// Called every frame
void AGridBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridBase::SetupOnLoad()
{
	float tile_Multiplier = 1.0f;

	switch (m_State)
	{
	case Tilled:
		tile_Multiplier = 0.75f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);

		m_Tile->SetStaticMesh(m_PlantedMesh);
		break;
	case Watered:
		tile_Multiplier = 0.4f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);
		break;
	case WateredAndTilled:
		tile_Multiplier = 0.3f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);

		m_Tile->SetStaticMesh(m_PlantedMesh);
		break;
	case Planted:
		tile_Multiplier = 0.75f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);

		m_Tile->SetStaticMesh(m_PlantedMesh);

		if (m_Plant->GetStaticMesh() != nullptr)
		{
			// Add plant mesh to the top.
			m_Plant->SetActive(true);
		}
		break;
	case WateredAndPlanted:
		tile_Multiplier = 0.3f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);

		m_Tile->SetStaticMesh(m_PlantedMesh);

		if (m_Plant->GetStaticMesh() != nullptr)
		{
			// Add plant mesh to the top.
			m_Plant->SetActive(true);
		}
	}
}

void AGridBase::Interact(Equipment item)
{
	// Get the player character
	AFinalYearProjectCharacter* character = Cast<AFinalYearProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	float tile_Multiplier = 1.0f;

	switch (m_State)
	{
	case Empty:
		// Update tile multiplier to darken the material and give it a watered look.
		if (item == Equipment::Watering_Can)
		{
			UE_LOG(LogTemp, Display, TEXT("Watering %s"), *GetName());

			tile_Multiplier = 0.4f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);
			m_State = Watered;
		}
		else if (item == Equipment::Rake)
		{
			UE_LOG(LogTemp, Display, TEXT("Tilling %s"), *GetName());

			tile_Multiplier = 0.75f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);

			m_Tile->SetStaticMesh(m_PlantedMesh);
			m_State = Tilled;
		}
		break;
	case Tilled:
		// Update tile multiplier to darken the material and give it a watered look.
		if (item == Equipment::Watering_Can)
		{
			UE_LOG(LogTemp, Display, TEXT("Watering %s"), *GetName());

			tile_Multiplier = 0.3f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);
			m_State = WateredAndTilled;
		}
		else if (item == Equipment::Seeds)
		{
			if (m_Plant->GetStaticMesh() != nullptr)
			{
				UE_LOG(LogTemp, Display, TEXT("Planting %s"), *GetName());

				// Add plant mesh to the top.
				m_Plant->SetActive(true);
				m_State = Planted;
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("No mesh, couldn't plant"));
			}
		}
		break;
	case Watered:
		if (item == Equipment::Rake)
		{
			UE_LOG(LogTemp, Display, TEXT("Tilling %s"), *GetName());

			tile_Multiplier = 0.3f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);

			m_Tile->SetStaticMesh(m_PlantedMesh);
			m_State = WateredAndTilled;
		}
		break;
	case WateredAndTilled:
		if (item == Equipment::Seeds)
		{
			if (m_Plant->GetStaticMesh() != nullptr)
			{
				UE_LOG(LogTemp, Display, TEXT("Planting %s"), *GetName());

				// Add plant mesh to the top.
				m_Plant->SetActive(true);

				m_State = WateredAndPlanted;
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("No mesh, couldn't plant"));
			}
		}
		break;
	case Planted:
		UE_LOG(LogTemp, Display, TEXT("Planted State: Plant = %s, Growth Time = %d"), *GetPlantName().ToString(), m_GrowthTime);
		if (m_GrowthTime <= 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Harvesting %s"), *GetName());

			// Reset tile to tilled state
			tile_Multiplier = 0.75f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);
			m_Tile->SetStaticMesh(m_PlantedMesh);
			m_State = Tilled;

			// Add plant to inventory
			character->AddToInventory(*m_GameInstance->GetSeedData(m_CurrentPlantName), ItemType::Crop);

			// Reset plant mesh afterwards
			m_Plant->SetStaticMesh(nullptr);
			m_Plant->SetActive(false);
			m_CurrentPlantName = NAME_None;
		}
		else if (item == Equipment::Watering_Can)
		{
			UE_LOG(LogTemp, Display, TEXT("Watering %s"), *GetName());

			tile_Multiplier = 0.3f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);
			m_State = WateredAndPlanted;
		}
		break;
	case WateredAndPlanted:
		UE_LOG(LogTemp, Display, TEXT("Watered and Planted State: Plant = %s, Growth Time = %d"), *GetPlantName().ToString(), GetGrowthTime());
		if(m_GrowthTime <= 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Harvesting %s"), *GetName());

			// Reset tile to tilled state
			tile_Multiplier = 0.75f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);
			m_Tile->SetStaticMesh(m_PlantedMesh);
			m_State = Tilled;

			// Add plant to inventory
			character->AddToInventory(*m_GameInstance->GetSeedData(m_CurrentPlantName), ItemType::Crop);

			// Reset plant mesh afterwards
			m_Plant->SetStaticMesh(nullptr);
			m_Plant->SetActive(false);
			m_CurrentPlantName = NAME_None;
		}
		break;
	default:
		break;
	}

}

void AGridBase::SetPlantMesh(UStaticMesh* mesh)
{
	if (m_State == Tilled || m_State == WateredAndTilled)
	{
		UE_LOG(LogTemp, Display, TEXT("SetPlantMesh"));

		m_Plant->SetStaticMesh(mesh);
		m_Plant->SetRelativeScale3D(FVector(7.0, 7.0, 7.0));
	}
}

void AGridBase::SetCurrentPlantFromName(FName plant)
{
	if (plant == NAME_None)
		m_CurrentPlantName = NAME_None;
	else
	{
		m_CurrentPlantName = plant;

		FSeedData* data = m_GameInstance->GetSeedData(plant);

		// Update the seed mesh at runtime
		m_Plant->SetStaticMesh(Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *(data->AssetPath))));

		m_GrowthTime = data->GrowthTime;

		// setup growth scales
		FVector maxScale = FVector(data->MaxSizeXY, data->MaxSizeXY, data->MaxSizeZ);
		FVector minScale = FVector(data->StartingSizeXY, data->StartingSizeXY, data->StartingSizeZ);
		for (int i = 0; i <= m_GrowthTime; i++)
		{
			FVector scale = maxScale;
			float alpha = float(i) / float(m_GrowthTime);

			scale.X = FMath::Lerp(maxScale.X, minScale.X, alpha);
			scale.Y = FMath::Lerp(maxScale.Y, minScale.Y, alpha);
			scale.Z = FMath::Lerp(maxScale.Z, minScale.Z, alpha);

			UE_LOG(LogTemp, Display, TEXT("%s has scale %d (alpha %f) as (%f, %f, %f)"), *m_CurrentPlantName.ToString(), i, alpha, scale.X, scale.Y, scale.Z);

			m_GrowthScales.Add(scale);
		}

		SetPlantScale();
	}
}

void AGridBase::SetPlantScale()
{
	// TODO: Work out where to call this
	FVector scale = m_GrowthScales[m_GrowthTime];
	m_Plant->SetRelativeScale3D(scale);

	UE_LOG(LogTemp, Display, TEXT("setting plant scale"));
}

void AGridBase::NextDayUpdate()
{
	float tile_Multiplier = 1.0f;

	switch (m_State)
	{
	case Tilled:
		// become untilled.
		m_Tile->SetStaticMesh(m_UnplantedMesh);
	case Watered:
		// become unwatered.
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);
		m_State = Empty;
		break;
	case WateredAndPlanted:
		// plant grows and becomes unwatered.
		m_GrowthTime--;
		SetPlantScale();
		tile_Multiplier = 0.75f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);
		m_State = Planted;
		break;
	case WateredAndTilled:
		// become unwatered.
		tile_Multiplier = 0.75f;
		m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
		m_Tile->SetMaterial(0, m_GridMaterial);
		m_State = Tilled;
		break;
	case Empty:
	case Planted:
	default:
		break;
	}
}