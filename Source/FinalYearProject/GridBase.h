// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalYearProjectCharacter.h"
#include "Plant.h"
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

	UPROPERTY(VisibleAnywhere, Category = Seed)
	class UStaticMeshComponent* m_Plant;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Materials)
	class UMaterialInterface* m_HighlightMaterial;

	class UMaterialInstanceDynamic* m_GridMaterial;

	// State of the farm tile.
	enum State {
		Empty = 0,
		Tilled = 1,
		Watered = 2,
		WateredAndTilled = 3,
		Planted = 4,
		WateredAndPlanted = 5
	};

	State m_State;

	UStaticMesh* m_UnplantedMesh;
	UStaticMesh* m_PlantedMesh;

	APlant* m_CurrentPlant;
	
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

	void Interact(Equipment item);

	void SetPlantMesh(UStaticMesh* mesh);

	void SetCurrentPlant(APlant* plant);
};
