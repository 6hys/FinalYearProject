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
	// Icon Image
	// Amount
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	FString RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	FString SeedName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int GrowthTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	FString AssetPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	FString ImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Seeds)
	int Amount;

	// overload == operator
	bool operator==(const FSeedData& b) const;
};