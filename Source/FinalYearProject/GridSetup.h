// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridBase.h"
#include "GridSetup.generated.h"

UCLASS()
class FINALYEARPROJECT_API AGridSetup : public AActor
{
	GENERATED_BODY()

	const int m_GridX = 10;
	const int m_GridY = 10;
	const int m_GridSize = 200;

	UPROPERTY(EditAnywhere, Category = Grid)
	TArray<AGridBase*> m_GridArray;

	UPROPERTY(EditAnywhere, Category = Scene)
	USceneComponent* m_NewRoot;
	
public:	
	// Sets default values for this actor's properties
	AGridSetup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Get the grid array
	FORCEINLINE TArray<AGridBase*> GetGridArray() { return m_GridArray; }
};
