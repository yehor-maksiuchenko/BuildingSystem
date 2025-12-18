// Copyright Epic Games, Inc. All Rights Reserved.

#include "BuildingSystemCharacter.h"
#include "BuildingSystemProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABuildingSystemCharacter

ABuildingSystemCharacter::ABuildingSystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetOwnerNoSee(false);

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetupAttachment(GetCapsuleComponent());
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = true;
	Mesh3P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	Mesh3P->SetOnlyOwnerSee(false);
	Mesh3P->SetOwnerNoSee(true);

	bReplicates = true;
	SetReplicateMovement(true);

	Mesh1P->SetIsReplicated(true);
	Mesh3P->SetIsReplicated(true);
	FirstPersonCameraComponent->SetIsReplicated(false);

	BuildComponent = CreateDefaultSubobject<UBuildComponent>(TEXT("BuildComponent"));
}

void ABuildingSystemCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void ABuildingSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABuildingSystemCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABuildingSystemCharacter::Look);

		EnhancedInputComponent->BindAction(ToggleBuildModeAction, ETriggerEvent::Started, BuildComponent, &UBuildComponent::ToggleBuildMode);
		EnhancedInputComponent->BindAction(PlaceTheTileAction, ETriggerEvent::Started, BuildComponent, &UBuildComponent::TryPlaceTheTile);
		EnhancedInputComponent->BindAction(NextTileAction, ETriggerEvent::Started, BuildComponent, &UBuildComponent::SelectNextTile);
		EnhancedInputComponent->BindAction(PreviousTileAction, ETriggerEvent::Started, BuildComponent, &UBuildComponent::SelectPreviousTile);
		EnhancedInputComponent->BindAction(RotatePreviewAction, ETriggerEvent::Started, BuildComponent, &UBuildComponent::RotatePreview);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ABuildingSystemCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABuildingSystemCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}