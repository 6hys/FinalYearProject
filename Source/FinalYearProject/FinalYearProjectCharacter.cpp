// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/Object.h"

#include "FinalYearProjectGameInstance.h"
#include "FinalYearProjectHUD.h"
#include "FinalYearProjectPlayerController.h"
#include "GridBase.h"
#include "UI_EndOfDayScreen.h"
#include "UI_Inventory.h"
#include "UI_RadialHUD.h"
#include "UI_SeedItem.h"
#include "UI_SellingInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFinalYearProjectCharacter

AFinalYearProjectCharacter::AFinalYearProjectCharacter(const FObjectInitializer& ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create the equipment mesh component
	FP_Equipment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FP_Equipment"));
	FP_Equipment->SetOnlyOwnerSee(true);	// only the player will see this.
	FP_Equipment->bCastDynamicShadow = false;
	FP_Equipment->CastShadow = false;
	FP_Equipment->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Equipment->SetupAttachment(RootComponent);
	FP_Equipment->SetActive(true);

	// Get the Watering Can mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> wateringCan(TEXT("/Game/LowPolyFarm/Meshes/Props/Mesh_Props_WateringCan_02.Mesh_Props_WateringCan_02"));
	if (wateringCan.Succeeded())
	{
		m_WateringCanMesh = wateringCan.Object;
	}

	// Get the Rake mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> rake(TEXT("/Game/LowPolyFarm/Meshes/Props/Mesh_Props_Rake_01.Mesh_Props_Rake_01"));
	if (rake.Succeeded())
	{
		m_RakeMesh = rake.Object;
	}

	// Initialise the seeds to null
	m_SeedMesh = nullptr;
	m_CurrentPlant = nullptr;

	// Get the radial menu HUD
	static ConstructorHelpers::FClassFinder<UUI_RadialHUD> radialHUD(TEXT("/Game/FirstPerson/UI/RadialMenu/RadialHUD"));
	if (radialHUD.Succeeded())
	{
		m_RadialHUDClass = radialHUD.Class;
	}

	// Get the hotbar
	static ConstructorHelpers::FClassFinder<UUI_Hotbar> hotbar(TEXT("/Game/FirstPerson/UI/Hotbar"));
	if(hotbar.Succeeded())
	{
		m_HotbarClass = hotbar.Class;
	}

	// Get the inventory
	static ConstructorHelpers::FClassFinder<UUI_Inventory> inventory(TEXT("/Game/FirstPerson/UI/Inventory"));
	if (inventory.Succeeded())
	{
		m_InventoryClass = inventory.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> popup(TEXT("/Game/FirstPerson/UI/EndDayPopup"));
	if (popup.Succeeded())
	{
		m_PopupClass = popup.Class;
	}

	// Get the end of day screen class.
	static ConstructorHelpers::FClassFinder<UUI_EndOfDayScreen> eodScreen(TEXT("/Game/FirstPerson/UI/EndOfDayScreen"));
	if (eodScreen.Succeeded())
	{
		m_EoDScreenClass = eodScreen.Class;
	}

	// Get the selling interface
	static ConstructorHelpers::FClassFinder<UUI_SellingInterface> sellingInterface(TEXT("/Game/FirstPerson/UI/SellingInterface"));
	if (sellingInterface.Succeeded())
	{
		m_SellingScreenClass = sellingInterface.Class;
	}

	m_CanJump = true;
	m_IsInventoryOpen = false;
	m_IsRadialOpen = false;
	m_CurrentlyEquipped = Equipment::Rake;
	m_CurrentOffset = FVector(0, 0, 0);
}

void AFinalYearProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Get the game instance
	m_GameInstance = Cast<UFinalYearProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Setup the controller 
	m_Controller = Cast<AFinalYearProjectPlayerController>(GetWorld()->GetFirstPlayerController());

	// Initialise player position
	SetActorLocation(m_GameInstance->GetLoadedPlayerLocation());
	// https://answers.unrealengine.com/questions/207242/view.html
	m_Controller->SetControlRotation(m_GameInstance->GetLoadedPlayerRotation());

	// Attach watering can mesh to character.
	FP_Equipment->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	FP_Equipment->SetStaticMesh(m_RakeMesh);
	m_CurrentOffset = FVector(-6.0, -3.0, 34.0);
	FP_Equipment->SetRelativeLocation(m_CurrentOffset);
	m_CurrentRotation = FRotator(338.0, 220.0, 0.0);
	FP_Equipment->SetRelativeRotation(m_CurrentRotation);

	Mesh1P->SetHiddenInGame(false, true);

	// Don't display in main menu
	if (GetWorld()->GetMapName() != FString("UEDPIE_0_MainMenu"))
	{
		// Create the hotbar
		if (m_HotbarClass)
		{
			m_Hotbar = CreateWidget<UUI_Hotbar>(GetWorld(), m_HotbarClass);
			m_Hotbar->AddToViewport(9999);
			Equip(m_GameInstance->GetLoadedEquip());
		}

		// Set inventory items
		m_SeedItems = m_GameInstance->GetLoadedSeedInventory();
		m_CropItems = m_GameInstance->GetLoadedCropInventory();

		FName plant = m_GameInstance->GetLoadedPlant();
		if(plant != NAME_None)
			ChangeSeeds(m_GameInstance->GetLoadedPlant());
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFinalYearProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFinalYearProjectCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFinalYearProjectCharacter::Interact);

	// Bind equip events
	PlayerInputComponent->BindAction<FEquipDelegate, AFinalYearProjectCharacter, Equipment>("Equip1", IE_Pressed, this, &AFinalYearProjectCharacter::Equip, Equipment::Rake);
	PlayerInputComponent->BindAction<FEquipDelegate, AFinalYearProjectCharacter, Equipment>("Equip2", IE_Pressed, this, &AFinalYearProjectCharacter::Equip, Equipment::Watering_Can);
	PlayerInputComponent->BindAction<FEquipDelegate, AFinalYearProjectCharacter, Equipment>("Equip3", IE_Pressed, this, &AFinalYearProjectCharacter::Equip, Equipment::Seeds);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFinalYearProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFinalYearProjectCharacter::MoveRight);

	// Bind pause event
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AFinalYearProjectCharacter::Pause);

	// Bind radial menu events
	PlayerInputComponent->BindAction("RadialMenu", IE_Pressed, this, &AFinalYearProjectCharacter::OpenRadialMenu);
	PlayerInputComponent->BindAction("RadialMenu", IE_Released, this, &AFinalYearProjectCharacter::CloseRadialMenu);

	// Bind inventory event
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AFinalYearProjectCharacter::ToggleInventory);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFinalYearProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFinalYearProjectCharacter::LookUpAtRate);
}

void AFinalYearProjectCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFinalYearProjectCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFinalYearProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFinalYearProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFinalYearProjectCharacter::Interact()
{
	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	// https://www.youtube.com/watch?v=GfsnOiwKJcY&ab_channel=DevEnabled

	FVector start;
	FRotator rot;
	FHitResult hit;

	m_Controller->GetPlayerViewPoint(start, rot);

	// 2000 is the distance it checks for. Need to set this much lower. 
	FVector end = start + (rot.Vector() * 300);

	FCollisionQueryParams traceParams;
	GetWorld()->LineTraceSingleByObjectType(hit, start, end, ECC_GameTraceChannel2, traceParams);

	// Draw debug lines to show where I'm hitting - TEMP
	DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 2.0f);
	if (hit.GetActor())
	{
		// Interact with the tile
		AGridBase *hitTile = Cast<AGridBase>(hit.GetActor());
		// Don't update the tiles plant if its already been planted.
		if (m_CurrentlyEquipped == Seeds && m_SeedMesh != nullptr && hitTile->GetState() < 4 )
		{
			hitTile->SetPlantMesh(m_SeedMesh);
			hitTile->SetCurrentPlant(m_CurrentPlant);
		}
		hitTile->Interact(m_CurrentlyEquipped);
	}
	else
	{
		// Not aiming at any tiles
		UE_LOG(LogTemp, Display, TEXT("Interacting with nothing"));
	}
}

void AFinalYearProjectCharacter::Equip(Equipment newEquip)
{
	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	if (m_CurrentlyEquipped != newEquip)
	{
		switch (newEquip)
		{
		case Rake:
			// Equip rake.
			UE_LOG(LogTemp, Display, TEXT("Equipping Rake"));
			FP_Equipment->SetStaticMesh(m_RakeMesh);
			m_CurrentOffset = FVector(-6.0, -3.0, 34.0);
			m_CurrentRotation = FRotator(338.0, 220.0, 0.0);
			break;
		case Watering_Can:
			// Equip watering can
			UE_LOG(LogTemp, Display, TEXT("Equipping Watering Can"));
			FP_Equipment->SetStaticMesh(m_WateringCanMesh);
			m_CurrentOffset = FVector(0, 19, -15);
			m_CurrentRotation = FRotator(0.0f);
			break;
		case Seeds:
			// Equip seeds if possible
			UE_LOG(LogTemp, Display, TEXT("Equipping Seeds"));
			if (m_SeedMesh != nullptr)
			{
				FP_Equipment->SetStaticMesh(m_SeedMesh);
			}
			else
			{
				FP_Equipment->SetStaticMesh(NULL);
			}
			m_CurrentOffset = FVector(0, 19, -15);
			m_CurrentRotation = FRotator(0.0f);
			break;
		}

		// Update equipment position
		FP_Equipment->SetRelativeLocation(m_CurrentOffset);
		FP_Equipment->SetRelativeRotation(m_CurrentRotation);

		// Update the hotbar 
		m_Hotbar->SetSelected(int(newEquip));

		// Update the current equip
		m_CurrentlyEquipped = newEquip;
	}
}

void AFinalYearProjectCharacter::Pause()
{
	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	// Get the hud so we can pause the game
	if (m_Controller)
	{
		AFinalYearProjectHUD* hud = Cast<AFinalYearProjectHUD>(m_Controller->GetHUD());

		if (hud)
			hud->SetPaused(true);

		m_Controller->SetPause(true);
		m_Controller->bShowMouseCursor = true;
	}

	if(m_Hotbar)
		m_Hotbar->RemoveFromParent();
}

void AFinalYearProjectCharacter::ChangeSeeds(FName name)
{
	// Create a new plant object for the seeds
	m_CurrentPlant = NewObject<APlant>();

	m_CurrentPlant->init(name);

	// Update the seed mesh at runtime
	m_SeedMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *(m_CurrentPlant->GetFilePath())));

	// Force the equipment to change in the players hands.
	if (m_CurrentlyEquipped == Equipment::Seeds)
	{
		UE_LOG(LogTemp, Display, TEXT("changing equipped seed mesh"));
		FP_Equipment->SetStaticMesh(m_SeedMesh);
	}
}

void AFinalYearProjectCharacter::RemoveSeeds()
{
	// Clear selected seeds
	m_SeedMesh = nullptr;
	m_CurrentPlant = nullptr;
}

void AFinalYearProjectCharacter::OpenRadialMenu()
{
	UE_LOG(LogTemp, Display, TEXT("Open Radial Menu"));

	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	// create new radial menu
	m_RadialHUD = CreateWidget<UUI_RadialHUD>(GetWorld(), m_RadialHUDClass);

	// Check the inventory isnt open
	if (!m_RadialHUD->RadialMenu->GetIsOpen() && m_IsInventoryOpen == false)
	{
		//TODO: initialise radial menu with items in inventory

		m_IsRadialOpen = true;

		// stop player movement
		m_Controller->SetIgnoreLookInput(true);
		m_Controller->SetIgnoreMoveInput(true);
		m_CanJump = false;

		// Set mouse to center of screen
		int X, Y;
		m_Controller->GetViewportSize(X, Y);
		m_Controller->SetMouseLocation(X / 2, Y / 2);

		// Set radial menu open and make the opacity 1
		m_RadialHUD->RadialMenu->SetIsOpen(true);
		m_RadialHUD->RadialMenu->SetOpacity(1.0f);

		// Add HUD to viewport
		m_RadialHUD->AddToViewport(9999);
	}
}

void AFinalYearProjectCharacter::CloseRadialMenu()
{
	UE_LOG(LogTemp, Display, TEXT("Close Radial Menu"));

	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	if (m_RadialHUD->RadialMenu->GetIsOpen())
	{
		// Set radial menu closed and make opacity 0 
		m_RadialHUD->RadialMenu->SetIsOpen(false);
		m_RadialHUD->RadialMenu->SetOpacity(0.0f);

		// Remove HUD from viewport
		m_RadialHUD->RemoveFromParent();

		// free player movement
		m_Controller->SetIgnoreLookInput(false);
		m_Controller->SetIgnoreMoveInput(false);
		m_Controller->SetInputMode(FInputModeGameOnly());
		m_CanJump = true;

		m_IsRadialOpen = false;

		FString currentSeed = m_RadialHUD->RadialMenu->GetCurrentItem()->GetName();

		ChangeSeeds(*currentSeed);
	}
}

void AFinalYearProjectCharacter::ToggleInventory()
{
	UE_LOG(LogTemp, Display, TEXT("Toggle Inventory"));

	// Do nothing on the main menu or with the radial menu open.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu") || m_RadialHUD->RadialMenu->GetIsOpen() == true) return;

	if (m_IsInventoryOpen)
	{
		// Close inventory
		m_Inventory->RemoveFromParent();

		UIClosed();

		EnableInput(m_Controller);

		m_IsInventoryOpen = false;
	}
	else
	{
		if (m_InventoryClass)
		{
			m_Inventory = CreateWidget<UUI_Inventory>(GetWorld(), m_InventoryClass);
		}

		if (m_Inventory)
		{
			// Refresh inventory items
			m_Inventory->Setup(m_SeedItems, m_CropItems);

			// Open inventory
			m_Inventory->AddToViewport(9999);

			UIOpened();

			FInputModeUIOnly inputMode;
			inputMode.SetWidgetToFocus(m_Inventory->GetCachedWidget());
			m_Controller->SetInputMode(inputMode);

			DisableInput(m_Controller);

			m_IsInventoryOpen = true;
		}
	}
}

void AFinalYearProjectCharacter::AddToInventory(FSeedData data, ItemType type)
{
	// Check if items already in inventory - Add to amount instead
	// Else add to the array.
	int i = 0;

	switch (type) {
	case Seed:
		i = m_SeedItems.Find(data);
		if (i != INDEX_NONE)
		{
			m_SeedItems[i].Amount++;
		}
		else
		{
			data.Amount = 1;
			m_SeedItems.Add(data);
		}
		break;
	case Crop:
		i = m_CropItems.Find(data);
		if (i != INDEX_NONE)
		{
			m_CropItems[i].Amount++;
		}
		else
		{
			data.Amount = 1;
			m_CropItems.Add(data);
		}
		break;
	default:
		break;
	}
}

void AFinalYearProjectCharacter::ShowEndScreen()
{
	m_EndScreen = CreateWidget<UUI_EndOfDayScreen>(GetWorld(), m_EoDScreenClass);

	m_EndScreen->SetTitleText(m_GameInstance->GetDayCount());

	m_EndScreen->AddToViewport(9999);

	UIOpened();

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(m_EndScreen->GetCachedWidget());
	m_Controller->SetInputMode(inputMode);

	DisableInput(m_Controller);
}

void AFinalYearProjectCharacter::HideEndScreen()
{
	m_EndScreen->RemoveFromParent();

	UIClosed();

	// reenable input
	EnableInput(m_Controller);
}

void AFinalYearProjectCharacter::Jump()
{
	if (m_CanJump)
	{
		Super::Jump();
	}
}

void AFinalYearProjectCharacter::OpenPopup()
{
	m_Popup = CreateWidget<UUserWidget>(GetWorld(), m_PopupClass);

	m_Popup->AddToViewport(9999);

	UIOpened();

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(m_Popup->GetCachedWidget());
	m_Controller->SetInputMode(inputMode);

	DisableInput(m_Controller);
}

void AFinalYearProjectCharacter::ClosePopup()
{
	m_Popup->RemoveFromParent();

	UIClosed();

	EnableInput(m_Controller);
}

void AFinalYearProjectCharacter::OpenSellScreen()
{
	m_SellingScreen = CreateWidget<UUI_SellingInterface>(GetWorld(), m_SellingScreenClass);
	m_SellingScreen->AddToViewport(9999);
	UIOpened();

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(m_SellingScreen->GetCachedWidget());
	m_Controller->SetInputMode(inputMode);

	DisableInput(m_Controller);
}

void AFinalYearProjectCharacter::CloseSellScreen()
{
	m_SellingScreen->RemoveFromParent();
	UIClosed();
	EnableInput(m_Controller);
}

void AFinalYearProjectCharacter::UIOpened()
{
	// Set mouse to center of screen
	int X, Y;
	m_Controller->GetViewportSize(X, Y);
	m_Controller->SetMouseLocation(X / 2, Y / 2);

	// stop player movement
	m_Controller->bShowMouseCursor = true;
	m_Controller->SetIgnoreLookInput(true);
	m_Controller->SetIgnoreMoveInput(true);

	// remove hotbar
	m_Hotbar->RemoveFromParent();
}

void AFinalYearProjectCharacter::UIClosed()
{
	// allow player movement
	m_Controller->bShowMouseCursor = false;
	m_Controller->SetIgnoreLookInput(false);
	m_Controller->SetIgnoreMoveInput(false);
	m_Controller->SetInputMode(FInputModeGameOnly());

	// create new hotbar
	m_Hotbar = CreateWidget<UUI_Hotbar>(GetWorld(), m_HotbarClass);

	m_Hotbar->AddToViewport(9999);
	m_Hotbar->SetSelected(int(m_CurrentlyEquipped));
}

bool AFinalYearProjectCharacter::CanOpen()
{
	return (m_IsRadialOpen == false && m_IsInventoryOpen == false);
}