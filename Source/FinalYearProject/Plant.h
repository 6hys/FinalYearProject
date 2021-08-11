// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "SeedData.h"
#include "Plant.generated.h"

UCLASS()
class FINALYEARPROJECT_API APlant : public AActor
{
	GENERATED_BODY()
	
	// Plant name
	FString m_Name;

	// Seed name
	FString m_SeedName;

	// Mesh file path
	FString m_FilePath;

	// Plant value
	int m_Value;

	// Plant price
	int m_Price;

	// Grow time
	int m_GrowthSpeed;

	UDataTable* m_SeedDataTable;

	FSeedData* m_SeedData;

public:	
	// Sets default values for this actor's properties
	APlant();

	void init(FName name);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetName();
	FString GetFilePath();
	int GetValue();
	int GetPrice();

	FORCEINLINE FSeedData* GetSeedData() { return m_SeedData; }
	// Get growth time
	FORCEINLINE int GetGrowthTime() { return m_GrowthSpeed; }
	// Reduce growth time
	FORCEINLINE void ReduceGrowthTime() { m_GrowthSpeed--; }
	// Manually set growth time
	FORCEINLINE void SetGrowthTime(int newTime) { m_GrowthSpeed = newTime; }
};
