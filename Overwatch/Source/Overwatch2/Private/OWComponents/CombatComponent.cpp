// Fill out your copyright notice in the Description page of Project Settings.


#include "OWComponents/CombatComponent.h"
#include "Overwatch2/Overwatch2Character.h"
#include "Character/Widowmaker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon/Weapon.h"
#include "Weapon/WidowGun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/OWPlayerController.h"
#include "TimerManager.h"
#include "HUD/OWHUD.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 550.f;
	AimWalkSpeed = 192.5f;

	bAiming = false;
	bFireButtonPressed = false;
	ChargeTime = 0.0f;
	MaxChargeTime = 0.9f;
	BaseDamage = 12;
	ChargedDamage = 120;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, bAiming);
	DOREPLIFETIME(UCombatComponent, ChargeTime);
	DOREPLIFETIME(UCombatComponent, CombatState);
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if (Character->GetFirstPersonCamera())
		{
			DefaultFOV = Character->GetFirstPersonCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("TickComponent is called"));
	if (Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		InterpFOV(DeltaTime);
	}

	if (bAiming)
	{
		HandleCharging(DeltaTime);
	}
}

void UCombatComponent::SetWeapon(class AWeapon* NewWeapon)
{
	Weapon = NewWeapon;
	NewWeapon->SetOwner(Character);
	//NewWeapon->
}

void UCombatComponent::SetVenom(class AWidowGun* NewVenom)
{
	Venom = NewVenom;
	NewVenom->SetOwner(Character);
}

void UCombatComponent::Reload()
{
	if (CombatState != ECombatState::ECS_Reloading)
	{
		ServerReload();
	}
}

void UCombatComponent::FinishReloading()
{
	if (Character == nullptr) return;
	if (Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Idle;
	}
}

void UCombatComponent::ServerReload_Implementation()
{
	if (Character == nullptr) return;
	CombatState = ECombatState::ECS_Reloading;
	HandleReload();
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
	case ECombatState::ECS_Reloading:
		HandleReload();
		break;
	default:
		break;
	}
}

void UCombatComponent::StartFireTimer()
{
	if (Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(
		FireTimer, this, &UCombatComponent::FireTimerFinished, Weapon->FireDelay
	);
}

void UCombatComponent::FireTimerFinished()
{
	bCanFire = true;
	if (bFireButtonPressed && Weapon->bVolley)
	{
		Fire();
	}
}

void UCombatComponent::HandleReload()
{
	Character->PlayReloadMontage();
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	//Weapon != nullptr;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;  // bIsAiming이 true면 MaxWalkSpeed를 AimWalkSpeed로 설정, false면 BaseWalkSpeed 
	}
	// 	if (Character && Character->IsLocallyControlled())
	// 	{
	// 		ServerSetAiming(bIsAiming);
	// 	}
	if (!bIsAiming)
	{
		ChargeTime = 0.0f;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;  // bIsAiming이 true면 MaxWalkSpeed를 AimWalkSpeed로 설정, false면 BaseWalkSpeed 
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if (bFireButtonPressed)
	{

//		FHitResult HitResult;
//		TraceUnderCrosshairs(HitResult);
		Fire();
		// Weapon->Fire();
	}
}

void UCombatComponent::Fire()
{
	if (bCanFire)
	{
		bCanFire = false;
		ServerFire(HitTarget);
		ChargeTime = 0.0f;
		if (ChargeTime == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("CHARGETIMERESET"));
		}
		StartFireTimer();
	}
}

void UCombatComponent::ServerFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MultiCastFire(TraceHitTarget);
}

void UCombatComponent::MultiCastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if (Character)
	{
		// Character->PlayFireMontage(bAiming);
		Weapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::EKeyPressed(bool bPressed)
{
	bEKeyPressed = bPressed;

	if (bEKeyPressed)
	{

		FHitResult HitResult;
	}
}

void UCombatComponent::ServerVenom_Implementation()
{
	
}

void UCombatComponent::InterpFOV(float DeltaTime)
{
	//	if (Weapon)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("2222"));
	//	}
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, Weapon->GetZoomedFOV(), DeltaTime, Weapon->GetZoomInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, UnZoomInterpSpeed);
	}
	if (Character && Character->GetFirstPersonCamera())
	{
		Character->GetFirstPersonCamera()->SetFieldOfView(CurrentFOV);
		//UE_LOG(LogTemp, Warning, TEXT("%d"), Character->GetFirstPersonCamera()->FieldOfView);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon is null in InterpFOV"));
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 조준선 위치 시야 중앙 (뷰포트 중앙)
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		// 시작 위치 : 일단은 화면 중앙
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * 50000.f;

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		///
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			//HitTarget = End;
		}
		else
		{
			//HitTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 30.f, 12, FColor::Red);
		}
		///
	}
}

void UCombatComponent::HandleCharging(float DeltaTime)
{
	if (bAiming)
	{
		ChargeTime += DeltaTime;
		if (ChargeTime > MaxChargeTime)
		{
			ChargeTime = MaxChargeTime;
		}
	}
}