// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Overwatch2/Overwatch2Character.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/OWPlayerController.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 플레이어와 충돌되면 안됨, 근접 weaponmesh로 할건지 sphere로 할건지 추후 수정 필요

	// 근접공격 시 사용할 수 있도록 결정 후 수정 필요
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);
	// SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::SpendRound()
{
	--Ammo;
	OwnerCharacter = OwnerCharacter == nullptr ? Cast<AOverwatch2Character>(GetOwner()) : OwnerCharacter;
	if (OwnerCharacter)
	{
		OwnerController = OwnerController == nullptr ? Cast<AOWPlayerController>(OwnerCharacter->Controller) : OwnerController;
		if (OwnerController)
		{
			// setHUDAmmo()같은 거 호출
		}
	}
}

void AWeapon::OnRep_Ammo()
{
	// 탄약은 복제돼서 안 빼도 됨
	OwnerCharacter = OwnerCharacter == nullptr ? Cast<AOverwatch2Character>(GetOwner()) : OwnerCharacter;
	if (OwnerCharacter)
	{
		OwnerController = OwnerController == nullptr ? Cast<AOWPlayerController>(OwnerCharacter->Controller) : OwnerController;
		if (OwnerController)
		{
			// setHUDAmmo()같은 거 호출
		}
	}

	// 위 코드들 리팩토링 할지 추후 결정 SetHUDAmmo() 만들고 combactcomponent newweapon->sethudammo()해서 서버에 탄약 설정해주기
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();

	OwnerCharacter = OwnerCharacter == nullptr ? Cast<AOverwatch2Character>(GetOwner()) : OwnerCharacter;
	if (OwnerCharacter)
	{
		OwnerController = OwnerController == nullptr ? Cast<AOWPlayerController>(OwnerCharacter->Controller) : OwnerController;
		if (OwnerController)
		{
			// setHUDAmmo()같은 거 호출
		}
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeapon::Fire(const FVector& HitTarget)
{
	UE_LOG(LogTemp, Warning, TEXT("fire!!"));
	
	SpendRound();
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		if (HasAuthority())
		{
			SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EWeaponState::EWS_Max:
		break;
	default:
		break;
	}
}
void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EWeaponState::EWS_Max:
		break;
	default:
		break;
	}
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda([&]() {
		Destroy();
		}), 3, false);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME(AWeapon, Ammo);
}