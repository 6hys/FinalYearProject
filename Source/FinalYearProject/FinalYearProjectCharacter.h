// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Plant.h"
#include "UI_Hotbar.h"
#include "FinalYearProjectCharacter.generated.h"

class UInputComponent;

UENUM()
enum Equipment {
	Rake = 0,
	Watering_Can = 1,
	Seeds = 2
};

UENUM()
enum ItemType
{
	Seed,
	Crop
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

	// End screen functions for blueprints
	UFUNCTION(BlueprintCallable, Category = "End of Day")
		void ShowEndScreen();

	UFUNCTION(BlueprintCallable, Category = "End of Day")
		void HideEndScreen();

	UFUNCTION(BlueprintCallable, Category = "End Of Day")
		void OpenPopup();

	UFUNCTION(BlueprintCallable, Category = "End Of Day")
		void ClosePopup();

	UFUNCTION(BlueprintCallable, Category = "End Of Day")
		void PopupToEndScreen();

	// sell screen functions
	UFUNCTION(BlueprintCallable, Category = "Selling")
		void OpenSellScreen();

	UFUNCTION(BlueprintCallable, Category = "Selling")
		void CloseSellScreen();

	UFUNCTION(BlueprintCallable, Category = "Selling")
		void SellCrop(FString name);

	// Inventory function
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool CanOpen();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Changing equipped item
	void Equip(Equipment newEquip);

	// Add to inventory 
	void AddToInventory(FSeedData data, ItemType type);

protected:
	virtual void BeginPlay();

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

	// Pause the game
	void Pause();

	// Radial menu functions
	void OpenRadialMenu();
	void CloseRadialMenu();

	// Change the equipped seed
	void ChangeSeeds(FName name);
	void RemoveSeeds();

	// Jump override to stop input
	virtual void Jump() override;

	// Functions for repeating aspects of opening/closing UI elements
	void UIOpened();
	void UIClosed();

	// Radial menu HUD
	class UUI_RadialHUD* m_RadialHUD;
	TSubclassOf<class UUI_RadialHUD> m_RadialHUDClass;
	bool m_IsRadialOpen;

	// Hotbar
	UUI_Hotbar* m_Hotbar;
	TSubclassOf<UUI_Hotbar> m_HotbarClass;

	// Inventory
	class UUI_Inventory* m_Inventory;
	TSubclassOf<class UUI_Inventory> m_InventoryClass;
	bool m_IsInventoryOpen;

	// Arrays to store the information about the items in the inventory
	TArray<FSeedData> m_SeedItems;
	TArray<FSeedData> m_CropItems;
	int m_Money;
	
	// End of day screen
	TSubclassOf<class UUI_EndOfDayScreen> m_EoDScreenClass;
	class UUI_EndOfDayScreen* m_EndScreen;

	// end day popup
	TSubclassOf<UUserWidget> m_PopupClass;
	UUserWidget* m_Popup;

	// selling menu
	TSubclassOf<class UUI_SellingInterface> m_SellingScreenClass;
	class UUI_SellingInterface* m_SellingScreen;

	class AFinalYearProjectPlayerController* m_Controller;

	class UFinalYearProjectGameInstance* m_GameInstance;

	// Block jump input on some menus
	bool m_CanJump;

	DECLARE_DELEGATE_OneParam(FEquipDelegate, Equipment);

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	// Returns the hotbar object
	FORCEINLINE class UUI_Hotbar* GetHotbar() { return m_Hotbar; }
	// Check if inventory is open
	FORCEINLINE bool IsInventoryOpen() { return m_IsInventoryOpen; }
	// Returns the inventory
	FORCEINLINE class UUI_Inventory* GetInventory() { return m_Inventory; }
	// Get whats currently equipped
	FORCEINLINE Equipment GetCurrentlyEquipped() { return m_CurrentlyEquipped; }
	// Get the current plant
	FORCEINLINE APlant* GetCurrentPlant() { return m_CurrentPlant; }
	// Set the current plant
	FORCEINLINE void SetCurrentPlant(APlant* newPlant) { m_CurrentPlant = newPlant; }
	// Make a new hotbar
	FORCEINLINE void MakeNewHotbar() { m_Hotbar = CreateWidget<UUI_Hotbar>(GetWorld(), m_HotbarClass); }
	// Get seed inventory data
	FORCEINLINE TArray<FSeedData> GetSeedInvData() { return m_SeedItems; }
	// Get crop inventory data
	FORCEINLINE TArray<FSeedData> GetCropInvData() { return m_CropItems; }
	// Get money
	FORCEINLINE int GetMoney() { return m_Money; }
};

