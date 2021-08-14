// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalYearProjectCharacter.h"
#include "Plant.h"
#include "GridBase.generated.h"

// State of the farm tile.
UENUM()
enum State {
	Empty = 0,
	Tilled = 1,
	Watered = 2,
	WateredAndTilled = 3,
	Planted = 4,
	WateredAndPlanted = 5
};

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
	
public:	
	// Sets default values for this actor's properties
	AGridBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupOnLoad();

	void Interact(Equipment item);

	void SetPlantMesh(UStaticMesh* mesh);

	void SetCurrentPlantFromName(FName plant);

	void SetPlantScale();

	void NextDayUpdate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	virtual void OnConstruction(const FTransform& Transform) override;

	class UMaterialInstanceDynamic* m_GridMaterial;

	State m_State;

	UStaticMesh* m_UnplantedMesh;
	UStaticMesh* m_PlantedMesh;

private:

	FName m_CurrentPlantName;
	int m_GrowthTime;
	TArray<FVector> m_GrowthScales;

	class UFinalYearProjectGameInstance* m_GameInstance;

public:	

	FORCEINLINE State GetState() { return m_State; }
	FORCEINLINE FName GetPlantName() { return m_CurrentPlantName; }
	FORCEINLINE int GetGrowthTime() { return m_GrowthTime; }

	FORCEINLINE void SetState(State state) { m_State = state; }
	FORCEINLINE void SetGrowthTime(int time) { m_GrowthTime = time; }
};
