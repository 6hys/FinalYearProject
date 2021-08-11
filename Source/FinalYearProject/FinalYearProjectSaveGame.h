// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GridBase.h"
#include "SeedData.h"
#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectSaveGame.generated.h"

USTRUCT()
struct FCropGridData
{
	GENERATED_USTRUCT_BODY()

public:
	FCropGridData()
		: crop(NAME_None),
		  state(State::Empty),
		  growth(0)
	{
	}

	UPROPERTY()
	FName crop;

	UPROPERTY()
	TEnumAsByte<State> state;

	UPROPERTY()
	int growth;
};

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UFinalYearProjectSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFinalYearProjectSaveGame();

	// Name of the save file
	UPROPERTY()
	FString SaveSlotName;
	// User index
	UPROPERTY()
	int UserIndex;
	
	// Save info
	UPROPERTY()
	int Money;
	UPROPERTY()
	TArray<FSeedData> SeedInventory;
	UPROPERTY()
	TArray<FSeedData> CropInventory;
	UPROPERTY()
	int DayCounter;
	UPROPERTY()
	float CurrentTime;
	UPROPERTY()
	FVector PlayerLocation;
	UPROPERTY()
	FRotator PlayerRotation;
	UPROPERTY()
	TEnumAsByte<Equipment> CurrentEquip;
	UPROPERTY()
	FName CurrentPlant;
	UPROPERTY()
	TArray<FCropGridData> CropGrid;
};
