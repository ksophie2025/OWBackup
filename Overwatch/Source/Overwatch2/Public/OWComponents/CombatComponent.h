// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Overwatch2/CombatState.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OVERWATCH2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	friend class AOverwatch2Character;
	//friend class AWidowmaker;

	void SetWeapon(class AWeapon* NewWeapon);
	void SetVenom(class AWidowGun* NewVenom);
	void Reload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bAiming;

	UPROPERTY(Replicated, EditAnywhere)
	float ChargeTime;
	UPROPERTY(EditAnywhere)
	float MaxChargeTime;
	UPROPERTY(EditAnywhere)
	int32 BaseDamage;
	UPROPERTY(EditAnywhere)
	int32 ChargedDamage;

protected:
	virtual void BeginPlay() override;


	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	void FireButtonPressed(bool bPressed);

	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();  // 서버 클라 둘 다 발생하게


	UFUNCTION(Server, Reliable)
	void ServerVenom();

	void EKeyPressed(bool bPressed);


	UFUNCTION()
	void HandleCharging(float DeltaTime);

private:
	class AOverwatch2Character* Character;
	class AOWPlayerController* Controller;
	class AOWHUD* HUD;
	//class AWidowmaker* Widowmaker;
	UPROPERTY()
	class AWeapon* Weapon;
	UPROPERTY()
	class AWidowGun* Venom;

	// 위도우 기준
	UPROPERTY(VisibleAnywhere/*EditAnywhere*/) // 안 바꿀건데 혹시 임의로 바꾸면서 볼 수도 있으니 Edit~는 주석으로 일단 
	float BaseWalkSpeed;
	UPROPERTY(VisibleAnywhere/*EditAnywhere*/)
	float AimWalkSpeed;

	bool bFireButtonPressed;
	bool bEKeyPressed;

	FVector HitTarget;

	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 80.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 0.33f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float UnZoomInterpSpeed = 0.f;

	void InterpFOV(float DeltaTime);

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Idle;

	UFUNCTION()
	void OnRep_CombatState();


	// 연사 관련
	FTimerHandle FireTimer;

	// weapon.h로 이동
//	UPROPERTY(EditAnywhere, Category = "Combat")
//	float FireDelay = 0.1f;
	
//	UPROPERTY(EditAnywhere, Category = "Combat")
//	bool bVolley = true;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();
};
