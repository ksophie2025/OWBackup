// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Overwatch2/Overwatch2Character.h"
#include "Widowmaker.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AWidowmaker : public AOverwatch2Character
{
	GENERATED_BODY()

	///** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputMappingContext* DefaultMappingContext;

	///** Aim Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* AimAction;
	
public:
	AWidowmaker();
	//virtual void PostInitializeComponents() override;  // component 접근 가능


	//bool bIsAiming();

protected:
	// APawn interface
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void BeginPlay() override;

	//void AimButtonPressed();
	//void AimButtonReleased();

private:
	//UPROPERTY(VisibleAnywhere, Category = "Weapon")
	//class UStaticMeshComponent* WeaponComp;

	//UPROPERTY(VisibleAnywhere)
	//class UCombatComponent* Combat;
};
