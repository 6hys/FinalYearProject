// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalYearProjectPlayerController.h"

AFinalYearProjectPlayerController::AFinalYearProjectPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Call tick every frame.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AFinalYearProjectPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}