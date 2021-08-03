// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "SeedData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSeedData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FSeedData()
	{

	};

	/* Data I Need
	// Name
	// Price
	// Value
	// Days to Grow
	// Asset path
	// Initial size?
	// Growth multiplier?
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int GrowthTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	FString AssetPath;
};