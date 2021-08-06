// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectCharacter.h"

#include "Animation/AnimInstance.h"
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

#include "FinalYearProjectHUD.h"
#include "FinalYearProjectPlayerController.h"
#include "GridBase.h"
#include "UI_Hotbar.h"
#include "UI_Inventory.h"
#include "UI_RadialHUD.h"
#include "UI_SeedItem.h"

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
	m_IsInventoryOpen = false;

	m_CurrentlyEquipped = Equipment::None;
	m_CurrentOffset = FVector(0, 0, 0);
}

void AFinalYearProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Setup the controller 
	m_Controller = Cast<AFinalYearProjectPlayerController>(GetWorld()->GetFirstPlayerController());

	// Attach watering can mesh to character.
	FP_Equipment->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	FP_Equipment->SetStaticMesh(m_RakeMesh);
	m_CurrentOffset = FVector(-6.0, -3.0, 34.0);
	FP_Equipment->SetRelativeLocation(m_CurrentOffset);
	m_CurrentRotation = FRotator(338.0, 220.0, 0.0);
	FP_Equipment->SetRelativeRotation(m_CurrentRotation);

	Mesh1P->SetHiddenInGame(false, true);

	// TEMP
	ChangeSeeds(FName(TEXT("Carrot")));

	// Don't display in pause menu
	if (GetWorld()->GetMapName() != FString("UEDPIE_0_MainMenu"))
	{
		// Create the radial menu HUD
		if (m_RadialHUDClass)
		{
			m_RadialHUD = CreateWidget<UUI_RadialHUD>(GetWorld(), m_RadialHUDClass);
		}

		// Create the hotbar
		if (m_HotbarClass)
		{
			m_Hotbar = CreateWidget<UUI_Hotbar>(GetWorld(), m_HotbarClass);
			m_Hotbar->AddToViewport(9999);
		}

		if (m_InventoryClass)
		{
			m_Inventory = CreateWidget<UUI_Inventory>(GetWorld(), m_InventoryClass);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFinalYearProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFinalYearProjectCharacter::Interact);

	// Bind equip events
	PlayerInputComponent->BindAction<FEquipDelegate, AFinalYearProjectCharacter, Equipment>("Equip1", IE_Pressed, this, &AFinalYearProjectCharacter::Equip, Equipment::None);
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

	GetController()->GetPlayerViewPoint(start, rot);

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
		if (m_CurrentlyEquipped == Seeds && m_SeedMesh != nullptr)
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
		case None:
			// Not holding anything. This is used for tilling the ground currently.
			UE_LOG(LogTemp, Display, TEXT("Equipping None"));
			FP_Equipment->SetStaticMesh(m_RakeMesh);
			FP_Equipment->SetActive(true);
			m_CurrentOffset = FVector(-6.0, -3.0, 34.0);
			FP_Equipment->SetRelativeLocation(m_CurrentOffset);
			m_CurrentRotation = FRotator(338.0, 220.0, 0.0);
			FP_Equipment->SetRelativeRotation(m_CurrentRotation);
			m_Hotbar->SetSelected(int(None));
			break;
		case Watering_Can:
			// Equip watering can
			UE_LOG(LogTemp, Display, TEXT("Equipping Watering Can"));
			FP_Equipment->SetActive(true);
			FP_Equipment->SetStaticMesh(m_WateringCanMesh);
			m_CurrentOffset = FVector(0, 19, -15);
			FP_Equipment->SetRelativeLocation(m_CurrentOffset);
			FP_Equipment->SetRelativeRotation(FRotator(0.0f));
			m_Hotbar->SetSelected(int(Watering_Can));
			break;
		case Seeds:
			// Equip seeds if possible
			UE_LOG(LogTemp, Display, TEXT("Equipping Seeds"));
			FP_Equipment->SetActive(true);
			if (m_SeedMesh != nullptr)
			{
				FP_Equipment->SetStaticMesh(m_SeedMesh);
			}
			else
			{
				FP_Equipment->SetStaticMesh(NULL);
			}
			m_CurrentOffset = FVector(0, 19, -15);
			FP_Equipment->SetRelativeLocation(m_CurrentOffset);
			FP_Equipment->SetRelativeRotation(FRotator(0.0f));
			m_Hotbar->SetSelected(int(Seeds));
			break;
		}

		// Update the current equip
		m_CurrentlyEquipped = newEquip;
	}
}

void AFinalYearProjectCharacter::Pause()
{
	// Do nothing on the main menu.
	if (GetWorld()->GetMapName() == FString("UEDPIE_0_MainMenu")) return;

	// Get the hud so we can pause the game
	AFinalYearProjectHUD* hud = Cast<AFinalYearProjectHUD>(m_Controller->GetHUD());

	hud->SetPaused(true);
	m_Controller->SetPause(true);
	m_Controller->bShowMouseCursor = true;

	m_Hotbar->RemoveFromViewport();
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

	// Check the inventory isnt open
	if (!m_RadialHUD->RadialMenu->GetIsOpen() && m_IsInventoryOpen == false)
	{
		// stop player movement
		m_Controller->SetIgnoreLookInput(true);
		m_Controller->SetIgnoreMoveInput(true);

		// Set mouse to center of screen
		int X, Y;
		m_Controller->GetViewportSize(X, Y);
		m_Controller->SetMouseLocation(X / 2, Y / 2);

		// Play animation?
		m_RadialHUD->PlayAnimation(m_RadialHUD->InOut);

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
		// Remove HUD from viewport
		m_RadialHUD->RemoveFromViewport();

		// free player movement
		m_Controller->SetIgnoreLookInput(false);
		m_Controller->SetIgnoreMoveInput(false);
		m_Controller->SetInputMode(FInputModeGameOnly());

		// play animation?
		 m_RadialHUD->PlayAnimation(m_RadialHUD->InOut, 0.0f, 1, EUMGSequencePlayMode::Reverse);

		// Set radial menu closed and make opacity 0 
		m_RadialHUD->RadialMenu->SetIsOpen(false);
		m_RadialHUD->RadialMenu->SetOpacity(0.0f);

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
		m_Inventory->RemoveFromViewport();

		// free player movement
		m_Controller->bShowMouseCursor = false;
		m_Controller->SetIgnoreLookInput(false);
		m_Controller->SetIgnoreMoveInput(false);
		m_Controller->SetInputMode(FInputModeGameOnly());

		m_Hotbar->AddToViewport(9999);
		m_Hotbar->SetSelected(m_CurrentlyEquipped);

		m_IsInventoryOpen = false;
	}
	else
	{
		// Open inventory
		m_Inventory->AddToViewport(9999);

		// Set mouse to center of screen
		int X, Y;
		m_Controller->GetViewportSize(X, Y);
		m_Controller->SetMouseLocation(X / 2, Y / 2);

		// stop player movement
		m_Controller->bShowMouseCursor = true;
		m_Controller->SetIgnoreLookInput(true);
		m_Controller->SetIgnoreMoveInput(true);

		m_Hotbar->RemoveFromViewport();

		m_IsInventoryOpen = true;
	}
}