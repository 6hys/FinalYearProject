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
	class UStaticMeshComponent* m_Plane;

	UPROPERTY(VisibleDefaultsOnly, Category = Box)
	class UBoxComponent* m_Box;
	
	UPROPERTY(EditAnywhere, Category = Materials)
	class UMaterialInterface* m_HighlightMaterial;

	class UMaterialInstanceDynamic* m_GridMaterialRef;
	
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
};