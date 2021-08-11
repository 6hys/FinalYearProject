// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSetup.h"
#include "Engine/World.h"
#include "FinalYearProjectGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridSetup::AGridSetup()
{
	m_NewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = m_NewRoot;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridSetup::BeginPlay()
{
	Super::BeginPlay();

	// Get the game instance 
	UFinalYearProjectGameInstance* gameInstance = Cast<UFinalYearProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Draw the grid of tiles
	for (int i = 1; i <= m_GridX; i++)
	{
		for (int j = 1; j <= m_GridY; j++)
		{
			float x = (i * m_GridSize) - ((m_GridX * m_GridSize) / 2);
			float y = (j * m_GridSize) - ((m_GridY * m_GridSize) / 2);

			FVector location = FVector(x, y, 0.0f) + m_NewRoot->GetRelativeLocation();
			FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters spawnInfo = FActorSpawnParameters();
			spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			m_GridArray.Add(GetWorld()->SpawnActor<AGridBase>(location, rotation, spawnInfo));
		}
	}

	if (gameInstance->GetCropGrid().Num() > 0)
	{
		for (int i = 0; i < m_GridArray.Num(); i++)
		{
			FCropGridData tileData = gameInstance->GetCropGrid()[i];
			m_GridArray[i]->SetCurrentPlantFromName(tileData.crop);
			m_GridArray[i]->SetGrowthTime(tileData.growth);
			m_GridArray[i]->SetState(tileData.state);
			m_GridArray[i]->SetupOnLoad();
		}
	}
}

// Called every frame
void AGridSetup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

