// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SeedData.h"
#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectSaveGame.generated.h"

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
	FVector PlayerLocation;
	UPROPERTY()
	FRotator PlayerRotation;
	UPROPERTY()
	int CurrentEquip;
	UPROPERTY()
	FName CurrentPlant;


	// TODO: field data and current time
};
