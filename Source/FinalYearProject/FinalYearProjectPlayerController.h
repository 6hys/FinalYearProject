// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FinalYearProjectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API AFinalYearProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Constructor
	AFinalYearProjectPlayerController(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
