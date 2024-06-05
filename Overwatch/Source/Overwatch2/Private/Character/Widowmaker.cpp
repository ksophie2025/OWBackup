// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Widowmaker.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OWComponents/CombatComponent.h"


AWidowmaker::AWidowmaker()
{
	//WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	//WeaponComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	//Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

}

//void AWidowmaker::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//	if (Combat)
//	{
//		Combat->Widowmaker = this;
//	}
//}


//void AWidowmaker::BeginPlay()
//{
//	Super::BeginPlay();

	//Add Input Mapping Context
//	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
//	{
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
//		{
//			Subsystem->AddMappingContext(DefaultMappingContext, 0);
//		}
//	}
//}

//void AWidowmaker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
	// Set up action bindings
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//// Moving
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOverwatch2Character::Move);

		//// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOverwatch2Character::Look);

		// Aim
		//EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AWidowmaker::AimButtonPressed);
		//EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AWidowmaker::AimButtonReleased);

//	}
//}

//bool AWidowmaker::bIsAiming()
//{
//	return (Combat && Combat->bAiming);
//}

//void AWidowmaker::AimButtonPressed()
//{
//}
//
//void AWidowmaker::AimButtonReleased()
//{
//}
