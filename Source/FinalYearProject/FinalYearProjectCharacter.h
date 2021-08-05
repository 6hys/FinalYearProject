// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"

#include "Plant.h"
#include "FinalYearProjectCharacter.generated.h"

class UInputComponent;

enum Equipment {
	None = 0,
	Watering_Can = 1,
	Seeds = 2
};

UCLASS(config = Game)
class AFinalYearProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;
	
	/** Equipment mesh */
	UPROPERTY(VisibleAnywhere, Category=Mesh)
	class UStaticMeshComponent* FP_Equipment;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	Equipment m_CurrentlyEquipped;

	UPROPERTY(VisibleAnywhere)
	APlant* m_CurrentPlant;

	UStaticMesh* m_WateringCanMesh;
	UStaticMesh* m_RakeMesh;
	UStaticMesh* m_SeedMesh;

	FVector m_CurrentOffset;
	FRotator m_CurrentRotation;

public:
	AFinalYearProjectCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.s
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	// Interaction with the farm tiles.
	void Interact();

	// Changing equipped item
	void Equip(Equipment newEquip);

	// Pause the game
	void Pause();

	// Radial menu functions
	void OpenRadialMenu();
	void CloseRadialMenu();

	// Change the equipped seed
	void ChangeSeeds(FName name);
	void RemoveSeeds();

	// Radial menu HUD
	UPROPERTY()
	class UUI_RadialHUD* m_RadialHUD;

	UPROPERTY()
	TSubclassOf<class UUI_RadialHUD> m_RadialHUDClass;

	// Hotbar
	class UUI_Hotbar* m_Hotbar;
	TSubclassOf<class UUI_Hotbar> m_HotbarClass;


	DECLARE_DELEGATE_OneParam(FEquipDelegate, Equipment);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

