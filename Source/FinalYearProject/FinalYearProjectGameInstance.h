// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "FinalYearProjectSaveGame.h"
#include "SeedData.h"
#include "FinalYearProjectGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINALYEARPROJECT_API UFinalYearProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFinalYearProjectGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		void NewGameInit();

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		void LoadGame(FString name);

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		void SetupLoadMenu(class UScrollBox* savesList);

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		void SetSaveName(FString name);

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		int SetCurrentDay();

	UFUNCTION(BlueprintCallable, Category = "Saving Loading")
		float SetCurrentTime();

	UFUNCTION(BlueprintCallable, Category = "End Of Day")
		void NextDaySetup();

	UFUNCTION(BlueprintCallable, Category = "End Of Day")
		void GetCurrentTime(float update);

protected:
	/** Called when the game starts. */
	virtual void Init() override;

private:
	// https://answers.unrealengine.com/questions/145598/is-there-a-way-to-get-all-savegames-in-bp.html
	/** returns a list of all save games in /Saved/SaveGames folder, without the .sav extension (filename only) */
	static TArray<FString> GetAllSaveGameSlotNames();

	// List of saved games
	TArray<FString> m_Saves;
	FString m_SaveName;

	TSubclassOf<class UUI_LoadGameItem> m_LoadGameItemClass;

	// sky sphere
	TSubclassOf<AActor> m_SkySphereClass;
	AActor* m_SkySphere;
	int m_DayCounter;
	float m_CurrentTime;

	// crop grid
	class AGridSetup* m_CropGridActor;

	class AFinalYearProjectCharacter* m_Character;

	// storing load data to be setup once the level has loaded and widgets/actors have been created
	TArray<FSeedData> m_SeedInv;
	TArray<FSeedData> m_CropInv;
	FVector m_PlayerLoc;
	FRotator m_PlayerRot;
	Equipment m_Equipment;
	FName m_CurrentPlant;
	TArray<FCropGridData> m_CropGrid;
	int m_Money;

public:
	// get loaded seed inventory
	FORCEINLINE TArray<FSeedData> GetLoadedSeedInventory() { return m_SeedInv; }
	// get loaded crop inventory
	FORCEINLINE TArray<FSeedData> GetLoadedCropInventory() { return m_CropInv; }
	// get player location
	FORCEINLINE FVector GetLoadedPlayerLocation() { return m_PlayerLoc; }
	// get player rotation
	FORCEINLINE FRotator GetLoadedPlayerRotation() { return m_PlayerRot; }
	// get equipment
	FORCEINLINE Equipment GetLoadedEquip() { return m_Equipment; }
	// get current plant selection
	FORCEINLINE FName GetLoadedPlant() { return m_CurrentPlant; }
	// get day count
	FORCEINLINE int GetDayCount() { return m_DayCounter; }
	// get crop grid
	FORCEINLINE TArray<FCropGridData> GetCropGrid() { return m_CropGrid; }
	// get money
	FORCEINLINE int GetMoney() { return m_Money; }
};
