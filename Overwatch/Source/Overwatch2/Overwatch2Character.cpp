// Copyright Epic Games, Inc. All Rights Reserved.

#include "Overwatch2Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerController/OWPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "Weapon/WidowGun.h"
#include "OWComponents/CombatComponent.h"
#include "Overwatch2/Overwatch2.h"
#include "GameMode/OWGameMode.h"
#include "TimerManager.h"
#include "PlayerState/OWPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Character/OWAnimInstance.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AOverwatch2Character

AOverwatch2Character::AOverwatch2Character()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirdPersonCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	ThirdPersonCamera->bAutoActivate = false;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FirstPersonCamera->SetRelativeLocation(FVector(0, 10, 0));
	FirstPersonCamera->SetRelativeRotation(FRotator(0, 90, -90));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->bAutoActivate = true;

	// bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	//WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	//WeaponComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	// 충돌 관련
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);

// 	Venom = CreateDefaultSubobject<USceneComponent>(TEXT("VenomMine"));
// 	Venom->SetupAttachment(GetMesh(), TEXT("VenomSocket"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AOverwatch2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartInit();

	// ctrl 키 눌렀을 때 앉고, 뗐을때 일어서기---------------------------crouch animation
	/*if (bCrouchActivated)
	{
		playerAnim = Cast<UOWAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bIsCrouched = true;
	}
	else
	{
		playerAnim = Cast<UOWAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bIsCrouched = false;
	}*/
	// ctrl 키 눌렀을 때 앉고, 뗐을때 일어서기---------------------------crouch animation 끝
}

void AOverwatch2Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
}

//void AOverwatch2Character::SpawnDefaultWeapon()
//{
//	AOWGameMode* OWGameMode = Cast<AOWGameMode>(UGameplayStatics::GetGameMode(this));  // GetGameMode는 서버가 아니라면 null return
//	UWorld* World = GetWorld();
//	if (OWGameMode && World && !bElimmed && DefaultWeaponClass)
//	{	
//		AWidowGun* StartingWeapon = World->SpawnActor<AWidowGun>(DefaultWeaponClass);
//		StartingWeapon->bDestroyWeapon = true;
		//if (Combat)
		//{
//			StartingWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("VenomSocket"));
//			StartingWeapon->SetOwner(this);
		//}
//	}
	// FActorSpawnParameters params;
	// 		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 		VenomMine = GetWorld()->SpawnActor<AWidowGun>(VenomClass, Venom->GetComponentLocation(), Venom->GetComponentRotation(), params);
	// 		if (VenomMine)
	// 		{
	// 			VenomMine->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("VenomSocket"));
	// 			VenomMine->SetOwner(this);
	// 			UE_LOG(LogTemp, Warning, TEXT("VenomMine successfully attached to Venom"));
	// 		}
//}

void AOverwatch2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//SpawnDefaultWeapon();

	//Add Input Mapping Context
	// if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	PlayerController = PlayerController == nullptr ? Cast<AOWPlayerController>(Controller) : PlayerController;
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		PlayerController->SetHUDHealth(Health, MaxHealth);
	}
//	if (AOWPlayerController* PlayerController = Cast<AOWPlayerController>(Controller))
//	{
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
//		{
//			Subsystem->AddMappingContext(DefaultMappingContext, 0);
//		}
		// PlayerController->SetHUDHealth(Health, MaxHealth);
//	}
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &AOverwatch2Character::ReceiveDamage);
	}

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (Weapon)
		{
			Weapon->SetWeaponState(EWeaponState::EWS_Equipped);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
			Weapon->SetOwner(this);
			if (Combat)
			{
				Combat->SetWeapon(Weapon);
				UE_LOG(LogTemp, Warning, TEXT("Weapon successfully set in CombatComponent"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Weapon!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponClass is not set!"));
	}
//	if (VenomClass)
//	{
// 		FActorSpawnParameters params;
// 		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

// 		VenomMine = GetWorld()->SpawnActor<AWidowGun>(VenomClass, Venom->GetComponentLocation(), Venom->GetComponentRotation(), params);
// 		if (VenomMine)
// 		{
// 			VenomMine->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("VenomSocket"));
// 			VenomMine->SetOwner(this);
// 			UE_LOG(LogTemp, Warning, TEXT("VenomMine successfully attached to Venom"));
// 		}
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 	VenomMine = GetWorld()->SpawnActor<AWidowGun>(VenomClass);
 		if (VenomMine)
 		{
 			VenomMine->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("VenomSocket"));
 			VenomMine->SetOwner(this);
 			if (Combat)
 			{
 				Combat->SetVenom(VenomMine);
 				UE_LOG(LogTemp, Warning, TEXT("Weapon successfully set in CombatComponent"));
 			}
 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Weapon!"));
// 		}

//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Failed to spawn VenomMine!"));
//		}
		//GetWorld()->SpawnActor<AWeapon>(WeaponClass, Venom->GetComponentLocation(), Venom->GetComponentRotation(), params);
		
//	}
}

void AOverwatch2Character::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("Fire !!!"));
// 	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
// 	if (AnimInstance && FireMontage)
// 	{
// 		AnimInstance->Montage_Play(FireMontage);
// 		FName SectionName;
// 		SectionName = bAiming ? FName("RifleAim") : FName("Riflerp");
// 		AnimInstance->Montage_JumpToSection(SectionName);
// 	}
}

void AOverwatch2Character::PlayHitReactMontage()
{
	if (Combat == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("HIT !!!"));
// 	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
// 	if (AnimInstance && FireMontage)
// 	{
// 		AnimInstance->Montage_Play(FireMontage);
// 		FName SectionName("FromFront");
// 		AnimInstance->Montage_JumpToSection(SectionName);
// 	}
}

void AOverwatch2Character::PlayReloadMontage()
{
	if (Combat == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{	
	 	AnimInstance->Montage_Play(ReloadMontage);
	 	FName SectionName("Reload");
	 	AnimInstance->Montage_JumpToSection(SectionName);
		UE_LOG(LogTemp, Warning, TEXT("RELOAD"));
	}
}

void AOverwatch2Character::PlayVenomMontage()
{
	if (Combat == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && VenomMontage)
	{
		AnimInstance->Montage_Play(VenomMontage);
		FName SectionName("VenomMine");
		AnimInstance->Montage_JumpToSection(SectionName);
		UE_LOG(LogTemp, Warning, TEXT("VenomMine"));
	}
}

void AOverwatch2Character::Elim()
{
	if (Combat && Combat->Weapon)
	{
		if (Combat->Weapon->bDestroyWeapon)
		{
			Combat->Weapon->Destroy();

		}
		else
		{
			Combat->Weapon->Dropped();
		}
		UE_LOG(LogTemp, Warning, TEXT("DROPWEAPON"));
	}
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer, this, &AOverwatch2Character::ElimTimerFinished, ElimDelay
	);
}

void AOverwatch2Character::MulticastElim_Implementation()
{
	bElimmed = true;
	// 몽타주 ?
	UE_LOG(LogTemp, Warning, TEXT("ELIMMINATION"));

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AOverwatch2Character::ElimTimerFinished()
{
	AOWGameMode* gm = GetWorld()->GetAuthGameMode<AOWGameMode>();
	if (gm)
	{
		gm->RequestRespawn(this, Controller);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOverwatch2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOverwatch2Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOverwatch2Character::Look);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AOverwatch2Character::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AOverwatch2Character::CrouchButtonPressed);

		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AOverwatch2Character::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AOverwatch2Character::AimButtonReleased);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AOverwatch2Character::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AOverwatch2Character::FireButtonReleased);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AOverwatch2Character::ReloadButtonPressed);

		// Venom Mine
		EnhancedInputComponent->BindAction(VenomAction, ETriggerEvent::Started, this, &AOverwatch2Character::EKeyPressed);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AOverwatch2Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverwatch2Character, Health);
}

void AOverwatch2Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOverwatch2Character::Look(const FInputActionValue& Value)
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

void AOverwatch2Character::CrouchButtonPressed(const FInputActionValue& Value)
{
	/*if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}*/
	bCtrl= Value.Get<bool>();

	if (bCtrl)
	{
		//bCrouchActivated = true;
		playerAnim->bIsCrouched = true;
	}
	else
	{
		//bCrouchActivated = false;
		playerAnim->bIsCrouched = false;
	}
	
}

void AOverwatch2Character::ReloadButtonPressed()
{
	if (Combat)
	{
		Combat->Reload();
	}
}

void AOverwatch2Character::OnRep_Health()
{
	UpdateHUDHealth();
	PlayHitReactMontage();
}

void AOverwatch2Character::UpdateHUDHealth()
{
	PlayerController = PlayerController == nullptr ? Cast<AOWPlayerController>(Controller) : PlayerController;
	if (PlayerController)
	{
		PlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

// 처음 score 0 뜨게
void AOverwatch2Character::StartInit()
{
	if (OWPlayerState  == nullptr)
	{
		OWPlayerState = GetPlayerState<AOWPlayerState>();
		if (OWPlayerState)
		{
			OWPlayerState->AddToScore(0.f);
		}
	}
}

bool AOverwatch2Character::bIsAiming()
{
	return (Combat && Combat->bAiming);
}


void AOverwatch2Character::AimButtonPressed()
{
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void AOverwatch2Character::AimButtonReleased()
{
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

void AOverwatch2Character::FireButtonPressed()
{
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void AOverwatch2Character::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void AOverwatch2Character::EKeyPressed()
{
}

void AOverwatch2Character::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);  // 체력은 클라한테만 호출, 데미지 받는 건 서버에서만
	UpdateHUDHealth();
	PlayHitReactMontage();

	if (Health == 0)
	{
		AOWGameMode* gm = GetWorld()->GetAuthGameMode<AOWGameMode>();
		if (gm)
		{
			PlayerController = PlayerController == nullptr ? Cast<AOWPlayerController>(Controller) : PlayerController;
			AOWPlayerController* AttackerController = Cast<AOWPlayerController>(InstigatorController);
			gm->PlayerEliminated(this, PlayerController, AttackerController);
		}
	}
}



//void AOverwatch2Character::MulticastHit_Implementation()
//{
//	PlayHitReactMontage();
//}

FVector AOverwatch2Character::GetHitTarget() const
{
	if (Combat == nullptr) return FVector();
	return Combat->HitTarget;
}

ECombatState AOverwatch2Character::GetCombatState() const
{
	if (Combat == nullptr) return ECombatState::ECS_MAX;
	return Combat->CombatState;
}
