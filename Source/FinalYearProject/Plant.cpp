// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"

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
	m_SeedData = m_SeedDataTable->FindRow<FSeedData>(name, ContextString);

	// Initialise info from the data table
	m_Name = name.ToString();
	m_SeedName = m_SeedData->SeedName;
	m_Price = m_SeedData->Price;
	m_Value = m_SeedData->Value;
	m_FilePath = m_SeedData->AssetPath;
	m_GrowthSpeed = m_SeedData->GrowthTime;

	// Scale values
	FVector maxScale = FVector(m_SeedData->MaxSizeXY, m_SeedData->MaxSizeXY, m_SeedData->MaxSizeZ);
	FVector minScale = FVector(m_SeedData->StartingSizeXY, m_SeedData->StartingSizeXY, m_SeedData->StartingSizeZ);
	for (int i = 0; i <= m_GrowthSpeed; i++)
	{
		FVector scale = maxScale;
		float alpha = float(i) / float(m_GrowthSpeed);

		scale.X = FMath::Lerp(maxScale.X, minScale.X, alpha);
		scale.Y = FMath::Lerp(maxScale.Y, minScale.Y, alpha);
		scale.Z = FMath::Lerp(maxScale.Z, minScale.Z, alpha);

		UE_LOG(LogTemp, Display, TEXT("%s has scale %d (alpha %f) as (%f, %f, %f)"), *name.ToString(), i, alpha, scale.X, scale.Y, scale.Z);

		m_GrowthScales.Add(scale);
	}
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



