// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Overwatch2/Overwatch2Character.h"
#include "Overwatch2/Overwatch2.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->bRotationFollowsVelocity = true; // 총알 속도 맞춰 회전,,,,
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		TracerComp = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionComp, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}
	if (HasAuthority())
	{
		// 충돌 이벤트 함수 서버에만 바인딩
		CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit)
{
	//AOverwatch2Character* OWCharacter = Cast<AOverwatch2Character>(OtherActor);
	//if (OWCharacter)
	//{
	//	OWCharacter->MulticastHit();
	//}

	Destroy();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, GetActorTransform());  // 자동 파괴 기본
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

