// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Plant.generated.h"

UCLASS()
class FINALYEARPROJECT_API APlant : public AActor
{
	GENERATED_BODY()
	
	// Plant name
	FString m_Name;

	// Mesh file path
	FString m_FilePath;

	// Plant value
	int m_Value;

	// Plant price
	int m_Price;

	// Grow time
	int m_GrowthSpeed;

	UDataTable* m_SeedDataTable;

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
};
