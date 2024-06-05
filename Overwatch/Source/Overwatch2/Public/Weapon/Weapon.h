// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	
	EWS_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class OVERWATCH2_API AWeapon : public AActor
{
	GENERATED_BODY()
	

public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	virtual void Fire(const FVector& HitTarget);
	void Dropped();
	
	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 80.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 0.33f;

	UPROPERTY(EditAnywhere)
	float UnZoomInterpSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bVolley = true;

	bool bDestroyWeapon = false;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class USphereComponent* SphereComp;  // box로 교체 가능

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo = 35;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();

	UPROPERTY()
	class AOverwatch2Character* OwnerCharacter;
	UPROPERTY()
	class AOWPlayerController* OwnerController;

public:
	// FORCEINLINE USphereComponent* GetSphereComp() const { return SphereComp; }
	// FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	void SetWeaponState(EWeaponState State);
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
	FORCEINLINE float GetUnZoomInterpSpeed() const { return UnZoomInterpSpeed; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
