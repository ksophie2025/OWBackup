// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OWAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API UOWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;


	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class AOverwatch2Character* OverwatchCharacter;

	//UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	//class AWidowmaker* WidowmakerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bElimmed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bRKeyPressed;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float Horizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float Vertical;

	UFUNCTION()                      
	void AnimNotify_ReloadFinished();
};
