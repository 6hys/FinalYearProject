// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridBase.generated.h"

UCLASS()
class FINALYEARPROJECT_API AGridBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Plane)
	class UStaticMeshComponent* m_Tile;

	UPROPERTY(VisibleDefaultsOnly, Category = Plane)
	class UStaticMeshComponent* m_Highlight;

	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	class UBoxComponent* m_Box;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Materials)
	class UMaterialInterface* m_HighlightMaterial;

	class UMaterialInstanceDynamic* m_GridMaterial;

	// State of the farm tile.
	enum State {
		Empty = 0,
		Watered = 1,
		Planted = 2,
		WateredAndPlanted = 3
	};

	State m_State;
	
public:	
	// Sets default values for this actor's properties
	AGridBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact();
};
