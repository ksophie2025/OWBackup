// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Overwatch2/CombatState.h"
#include "Overwatch2Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AWeapon;
class AWidowGun;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AOverwatch2Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** ThirdPerson camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCamera;

	/** FirstPerson camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Venom Mine Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VenomAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY()
	AWeapon* Weapon;

 	UPROPERTY(EditDefaultsOnly)
 	TSubclassOf<AWidowGun> VenomClass;
	UPROPERTY()
	AWidowGun* VenomMine;

public:
	AOverwatch2Character();
	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;  // component 접근 가능
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Venom;

	void PlayFireMontage(bool bAiming);
	void PlayHitReactMontage();
	void PlayReloadMontage();
	void PlayVenomMontage();

	// 플레이어 제거될 때 발생
	void Elim();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastHit();


	//void SpawnDefaultWeapon();

protected:
	// To add mapping context
	virtual void BeginPlay() override;

	// APawn interface
	

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void CrouchButtonPressed(const FInputActionValue& Value);
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void EKeyPressed();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	void UpdateHUDHealth();

	void StartInit();

	class AOWPlayerState* OWPlayerState;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* VenomMontage;

//	UPROPERTY(EditAnywhere, Category = Combat)
//	class UAnimMontage* DieMontage;

private:
	//UPROPERTY(VisibleAnywhere, Category = "Weapon")
	//class UStaticMeshComponent* WeaponComp;


	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int32 MaxHealth = 200;

	UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, Category = "Player Stats")
	int32 Health;

	UFUNCTION()
	void OnRep_Health();

	class AOWPlayerController* PlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditAnywhere)
	float ElimDelay = 3.f;

	void ElimTimerFinished();



	UPROPERTY(EditAnywhere)
	TSubclassOf<AWidowGun> DefaultWeaponClass;


public:
	bool bIsAiming();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return ThirdPersonCamera; }
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE	int32 GetHealth() const { return Health; }
	FORCEINLINE	int32 GetMaxHealth() const { return MaxHealth; }

	UPROPERTY()
	class UOWAnimInstance* playerAnim;

	bool bCrouchActivated;
	bool bCtrl;
};

