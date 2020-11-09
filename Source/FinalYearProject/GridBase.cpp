// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGridBase::AGridBase()
{
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	RootComponent = m_Box;

	m_Box->CreationMethod = EComponentCreationMethod::Native;
	m_Box->SetBoxExtent(FVector(64.0f, 64.0f, 1.0f));
	m_Box->SetCollisionProfileName(FName(TEXT("Custom")));
	m_Box->SetCollisionObjectType(ECC_GameTraceChannel2);

	m_Plane->SetVisibility(false);
	m_Plane->SetRelativeScale3D(FVector(1.275f, 1.275f, 1.0f));
	m_Plane->SetCollisionProfileName(FName(TEXT("NoCollision")));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGridBase::OnConstruction(const FTransform& Transform)
{
	if (m_HighlightMaterial)
	{
		m_GridMaterialRef = m_Plane->CreateDynamicMaterialInstance(0, m_HighlightMaterial);
	}
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

