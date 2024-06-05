// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AProjectileBullet : public AProjectile
{
	GENERATED_BODY()

public:
	

protected:

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit) override;
	
private:
// 	class AOverwatch2Character* Character;
// 
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
// 	class UCombatComponent* Combat;
	
};
