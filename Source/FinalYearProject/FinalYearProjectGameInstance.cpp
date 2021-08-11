// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalYearProjectGameInstance.h"

#include "Components/ScrollBox.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "Misc/Paths.h"
#include "UObject/ConstructorHelpers.h"

#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectPlayerController.h"
#include "GridSetup.h"
#include "Plant.h"
#include "UI_LoadGameItem.h"

UFinalYearProjectGameInstance::UFinalYearProjectGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Get the class to set up the loading screen later.
	static ConstructorHelpers::FClassFinder<UUI_LoadGameItem> loadGameItem(TEXT("/Game/FirstPerson/UI/LoadGameItems"));
	if (loadGameItem.Succeeded())
	{
		m_LoadGameItemClass = loadGameItem.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> skySphere(TEXT("/Game/FirstPerson/Blueprints/SkySphereTOD"));
	if (skySphere.Succeeded())
	{
		m_SkySphereClass = skySphere.Class;
	}

	m_DayCounter = 1;
	m_CurrentTime = 7.0f;
}

void UFinalYearProjectGameInstance::Init()
{
	Super::Init();

	//Setup default values in the case of a new game.
	m_PlayerLoc = FVector(1759.0f, 381.0f, 235.0f);
	m_PlayerRot = FRotator(0.0f, -160.0f, 0.0f);
	m_Equipment = Equipment::Rake;
	m_CurrentPlant = NAME_None;
}

void UFinalYearProjectGameInstance::LoadGame(FString name)
{
	m_Character = Cast<AFinalYearProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Load game here
	UE_LOG(LogTemp, Display, TEXT("Loading %s"), *name);
	UFinalYearProjectSaveGame* loadedGame = Cast<UFinalYearProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(name, 0));
	if (loadedGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"), *loadedGame->SaveSlotName);

		// Open the level
		UGameplayStatics::OpenLevel(GetWorld(), "Game");

		m_SaveName = name;

		// ===== Setup from the save file ===== //
		// inventory
		m_SeedInv = loadedGame->SeedInventory;
		m_CropInv = loadedGame->CropInventory;
		// character position
		m_PlayerLoc = loadedGame->PlayerLocation;
		m_PlayerRot = loadedGame->PlayerRotation;
		// current equipment
		m_Equipment = loadedGame->CurrentEquip;
		m_CurrentPlant = loadedGame->CurrentPlant;
		// day and time
		m_DayCounter = loadedGame->DayCounter;
		m_CurrentTime = loadedGame->CurrentTime;
		// field
		m_CropGrid = loadedGame->CropGrid;
	}
}

void UFinalYearProjectGameInstance::SaveGame()
{
	m_Character = Cast<AFinalYearProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_CropGridActor = Cast<AGridSetup>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridSetup::StaticClass()));

	UE_LOG(LogTemp, Display, TEXT("Saving game"));
	UFinalYearProjectSaveGame* saveGameInstance = Cast<UFinalYearProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UFinalYearProjectSaveGame::StaticClass()));
	if (saveGameInstance)
	{
		UE_LOG(LogTemp, Display, TEXT("Save instance valid"));
		// Set the save data here
		if (m_SaveName.IsEmpty() == false)
		{
			UE_LOG(LogTemp, Display, TEXT("saving as %s"), *m_SaveName);
			saveGameInstance->SaveSlotName = m_SaveName;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("no save name stored"));
		}
		// inventory saved
		saveGameInstance->SeedInventory = m_Character->GetSeedInvData();
		saveGameInstance->CropInventory = m_Character->GetCropInvData();
		// player position saved
		saveGameInstance->PlayerLocation = m_Character->GetActorLocation();
		saveGameInstance->PlayerRotation = m_Character->GetActorRotation();
		// equipment saved
		saveGameInstance->CurrentEquip = m_Character->GetCurrentlyEquipped();
		if (m_Character->GetCurrentPlant() != nullptr)
			saveGameInstance->CurrentPlant = FName(*m_Character->GetCurrentPlant()->GetName());
		else
			saveGameInstance->CurrentPlant = NAME_None;
		// day and time
		saveGameInstance->DayCounter = m_DayCounter;
		saveGameInstance->CurrentTime = m_CurrentTime;
		// field - MAYBE NEED TO SAVE ALL THE VARIABLES SEPARATELY INSTEAD
		// STATE, PLANT, GROWTH, SIZE???
		for (AGridBase* tile : m_CropGridActor->GetGridArray())
		{
			FCropGridData tileData;
			tileData.crop = tile->GetPlantName();
			tileData.state = tile->GetState();
			tileData.growth = tile->GetGrowthTime();
			
			saveGameInstance->CropGrid.Add(tileData);
		}
		//saveGameInstance->CropGrid = m_CropGridActor->GetGridArray();

		// https://docs.unrealengine.com/4.26/en-US/InteractiveExperiences/SaveGame/#:~:text=Unreal%20Engine%204%20(UE4)%20features,preserve%20across%20multiple%20play%20sessions.
		// Save game
		if (UGameplayStatics::SaveGameToSlot(saveGameInstance, saveGameInstance->SaveSlotName, saveGameInstance->UserIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Save successful"));
		}
	}
}

void UFinalYearProjectGameInstance::SetupLoadMenu(UScrollBox* savesList)
{
	m_Saves = GetAllSaveGameSlotNames();

	for (FString save : m_Saves)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *save);

		UUI_LoadGameItem* menuItem = CreateWidget<UUI_LoadGameItem>(GetWorld(), m_LoadGameItemClass);

		menuItem->SetSlotName(save);
		savesList->AddChild(menuItem);
	}
}

// https://answers.unrealengine.com/questions/145598/is-there-a-way-to-get-all-savegames-in-bp.html
// Gets all the save files
TArray<FString> UFinalYearProjectGameInstance::GetAllSaveGameSlotNames()
{
	//////////////////////////////////////////////////////////////////////////////
	class FFindSavesVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FFindSavesVisitor() {}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				FString FullFilePath(FilenameOrDirectory);
				if (FPaths::GetExtension(FullFilePath) == TEXT("sav"))
				{
					FString CleanFilename = FPaths::GetBaseFilename(FullFilePath);
					CleanFilename = CleanFilename.Replace(TEXT(".sav"), TEXT(""));
					SavesFound.Add(CleanFilename);
				}
			}
			return true;
		}
		TArray<FString> SavesFound;
	};
	//////////////////////////////////////////////////////////////////////////////

	TArray<FString> Saves;
	const FString SavesFolder = FPaths::ProjectSavedDir() + TEXT("SaveGames");

	if (!SavesFolder.IsEmpty())
	{
		FFindSavesVisitor Visitor;
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*SavesFolder, Visitor);
		Saves = Visitor.SavesFound;
	}

	return Saves;
}

void UFinalYearProjectGameInstance::SetSaveName(FString name)
{
	UE_LOG(LogTemp, Display, TEXT("save name =  %s"), *name);
	m_SaveName = name;
}

void UFinalYearProjectGameInstance::NextDaySetup()
{
	UE_LOG(LogTemp, Display, TEXT("setup test"));

	m_Character = Cast<AFinalYearProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// update day and time
	m_DayCounter++;
	m_CurrentTime = 7.0f;

	// set player position
	m_Character->SetActorLocation(FVector(-1325.5f, -950.0f, 218.0f));
	AFinalYearProjectPlayerController* controller = Cast<AFinalYearProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	controller->SetControlRotation(FRotator(0.0f, 26.0f, 0.0f));
	// also call this for the sake of save and quit
	m_Character->SetActorRotation(FRotator(0.0f, 26.0f, 0.0f));

	// Get the grid setup
	m_CropGridActor = Cast<AGridSetup>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridSetup::StaticClass()));
	// update crops status
	TArray<AGridBase*> cropGrid = m_CropGridActor->GetGridArray();
	for (AGridBase* tile : cropGrid)
	{
		tile->NextDayUpdate();
	}

	// set time to morning
	// https://answers.unrealengine.com/questions/606901/in-c-get-the-bp-sky-sphere.html
	m_SkySphere = UGameplayStatics::GetActorOfClass(GetWorld(), m_SkySphereClass);
	FOutputDeviceNull ar;
	m_SkySphere->CallFunctionByNameWithArguments(TEXT("NextDaySetup"), ar, NULL, true);
}

void UFinalYearProjectGameInstance::GetCurrentTime(float update)
{
	m_CurrentTime = update;
}

int UFinalYearProjectGameInstance::SetCurrentDay()
{
	return m_DayCounter;
}

float UFinalYearProjectGameInstance::SetCurrentTime()
{
	return m_CurrentTime;
}