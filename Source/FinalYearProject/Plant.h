// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

UCLASS()
class FINALYEARPROJECT_API APlant : public AActor
{
	GENERATED_BODY()

	// Plant name
	FName m_Name;

	// Number of sockets used.
	int m_Sockets;
	const FString c_SingleSocket = "Single_Plant";
	const FString c_MultipleSockets = "Multiple_Plants_";

	// Mesh file path
	FString m_FilePath;

public:	
	// Sets default values for this actor's properties
	APlant();
	APlant(FName name, int sockets, FString path);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetName();
	int GetSockets();
	FString GetSocketName();
	FString GetFilePath();
};
