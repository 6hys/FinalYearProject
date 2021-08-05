// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "UObject/ConstructorHelpers.h"
#include "SeedData.h"

// Empty constructor
APlant::APlant()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> seedDataTableObject(TEXT("DataTable'/Game/FirstPerson/Data/SeedData.SeedData'"));
	if (seedDataTableObject.Succeeded())
	{
		m_SeedDataTable = seedDataTableObject.Object;
	}

	m_Name = "Null";
	m_FilePath = "";

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Setup
void APlant::init(FName name)
{
	static const FString ContextString(TEXT("GENERAL"));
	FSeedData* rowLookUp = m_SeedDataTable->FindRow<FSeedData>(name, ContextString);

	// Initialise info from the data table
	m_Name = rowLookUp->Name;
	m_Price = rowLookUp->Price;
	m_Value = rowLookUp->Value;
	m_FilePath = rowLookUp->AssetPath;
	m_GrowthSpeed = rowLookUp->GrowthTime;
}

// Called when the game starts or when spawned
void APlant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString APlant::GetName()
{
	return m_Name;
}

FString APlant::GetFilePath()
{
	return m_FilePath;
}

int APlant::GetValue()
{
	return m_Value;
}

int APlant::GetPrice()
{
	return m_Price;
}



