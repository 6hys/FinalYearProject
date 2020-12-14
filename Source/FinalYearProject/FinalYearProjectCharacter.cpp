// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FinalYearProjectCharacter.h"
#include "FinalYearProjectProjectile.h"
#include "GridBase.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFinalYearProjectCharacter

AFinalYearProjectCharacter::AFinalYearProjectCharacter()
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> wateringCan((TEXT("/Game/LowPolyFarm/Meshes/Props/Mesh_Props_WateringCan_02.Mesh_Props_WateringCan_02")));
	if (wateringCan.Succeeded())
	{
		m_WateringCanMesh = wateringCan.Object;
	}

	m_CurrentlyEquipped = Equipment::None;
}

void AFinalYearProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Attach watering can mesh to character.
	FP_Equipment->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);
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
	PlayerInputComponent->BindAction("Equip1", IE_Pressed, this, &AFinalYearProjectCharacter::Equip1);
	PlayerInputComponent->BindAction("Equip2", IE_Pressed, this, &AFinalYearProjectCharacter::Equip2);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFinalYearProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFinalYearProjectCharacter::MoveRight);

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
	// https://www.youtube.com/watch?v=GfsnOiwKJcY&ab_channel=DevEnabled

	FVector start;
	FRotator rot;
	FHitResult hit;

	GetController()->GetPlayerViewPoint(start, rot);

	// 2000 is the distance it checks for. Need to set this much lower. 
	FVector end = start + (rot.Vector() * 250);

	FCollisionQueryParams traceParams;
	GetWorld()->LineTraceSingleByObjectType(hit, start, end, ECC_GameTraceChannel2, traceParams);

	DrawDebugLine(GetWorld(), start, end, FColor::Orange, false, 2.0f);
	if (hit.GetActor())
	{
		AGridBase *hitTile = Cast<AGridBase>(hit.GetActor());
		hitTile->Interact(m_CurrentlyEquipped);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Interacting with nothing"));
	}
}

void AFinalYearProjectCharacter::Equip1()
{
	Equip(Equipment::None);
}

void AFinalYearProjectCharacter::Equip2()
{
	Equip(Equipment::Watering_Can);
}

void AFinalYearProjectCharacter::Equip(Equipment newEquip)
{
	if (m_CurrentlyEquipped != newEquip)
	{
		switch (newEquip)
		{
		case None:
			UE_LOG(LogTemp, Display, TEXT("Equipping None"));
			FP_Equipment->SetStaticMesh(NULL);
			FP_Equipment->SetActive(false);
			FP_Equipment->ResetRelativeTransform();
			break;
		case Watering_Can:
			UE_LOG(LogTemp, Display, TEXT("Equipping Watering Can"));
			FP_Equipment->SetActive(true);
			FP_Equipment->SetStaticMesh(m_WateringCanMesh);
			FP_Equipment->AddLocalOffset(FVector(0, 19, -15));
			break;
		}

		m_CurrentlyEquipped = newEquip;
	}
}
