// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

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

	m_Tile->SetupAttachment(m_Box);
	m_Tile->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_Tile->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	m_Tile->SetCollisionProfileName(FName(TEXT("NoCollision")));
	m_Tile->CastShadow = false;

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

	// Initialise tile as empty.
	m_State = State::Empty;

	// Set mesh as unplanted
	m_Tile->SetStaticMesh(m_UnplantedMesh);

	// Get material
	m_GridMaterial = UMaterialInstanceDynamic::Create(m_Tile->GetMaterial(0), NULL);
	m_Tile->SetMaterial(0, m_GridMaterial);

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
	
}

// Called every frame
void AGridBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridBase::Interact(Equipment item)
{
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
		else if (item == Equipment::None)
		{
			UE_LOG(LogTemp, Display, TEXT("Tilling %s"), *GetName());

			tile_Multiplier = 0.75f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);

			m_Tile->SetStaticMesh(m_PlantedMesh);
			m_State = Planted;
		}
		break;
	case Planted:
		// Update tile multiplier to darken the material and give it a watered look.
		if (item == Equipment::Watering_Can)
		{
			UE_LOG(LogTemp, Display, TEXT("Watering %s"), *GetName());

			tile_Multiplier = 0.3f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);
			m_State = WateredAndPlanted;
		}
		break;
	case Watered:
		if (item == Equipment::None)
		{
			UE_LOG(LogTemp, Display, TEXT("Tilling %s"), *GetName());

			tile_Multiplier = 0.3f;
			m_GridMaterial->SetScalarParameterValue(TEXT("Multiplier"), tile_Multiplier);
			m_Tile->SetMaterial(0, m_GridMaterial);

			m_Tile->SetStaticMesh(m_PlantedMesh);
			m_State = WateredAndPlanted;
		}
	case WateredAndPlanted:
	default:
		break;
	}

}

