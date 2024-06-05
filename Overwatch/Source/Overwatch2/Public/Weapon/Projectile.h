// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.generated.h"

UCLASS()
class OVERWATCH2_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	int32 Damage = 12;

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;
	class UParticleSystemComponent* TracerComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactVFX;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;


public:
	
};
