// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "WidowGun.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AWidowGun : public AWeapon
{
	GENERATED_BODY()
	
	//UPROPERTY(EditAnywhere)
	//float ZoomedFOV = 30.f;

	//UPROPERTY(EditAnywhere)
	//float ZoomInterpSpeed = 0.33f;
public:
	virtual void Fire(const FVector& HitTarget) override;

protected:
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

private:
	UPROPERTY(EditAnywhere)
	float Damage = 100.f;
};
