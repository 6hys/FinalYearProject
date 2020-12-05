// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGridBase::AGridBase()
{
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	m_Highlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Highlight"));
	RootComponent = m_Box;

	m_Box->CreationMethod = EComponentCreationMethod::Native;
	m_Box->SetBoxExtent(FVector(64.0f, 64.0f, 1.0f));
	m_Box->SetCollisionProfileName(FName(TEXT("Custom")));
	m_Box->SetCollisionObjectType(ECC_GameTraceChannel2);

	m_Tile->SetupAttachment(m_Box);
	m_Tile->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_Tile->SetRelativeScale3D(FVector(0.6f, 0.6f, 1.0f));
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> dirtTile(TEXT("/Game/Geometry/Meshes/GridTile.GridTile"));
	if (dirtTile.Succeeded())
	{
		m_Tile->SetStaticMesh(dirtTile.Object);
	}

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

void AGridBase::Interact()
{
	UE_LOG(LogTemp, Display, TEXT("Interact with %s"), *GetName());
}

