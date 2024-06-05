// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OWAnimInstance.h"
#include "Overwatch2/Overwatch2Character.h"
// #include "Character/Widowmaker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Overwatch2/CombatState.h"

void UOWAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OverwatchCharacter = Cast<AOverwatch2Character>(TryGetPawnOwner());
}

void UOWAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OverwatchCharacter == nullptr)
	{
		OverwatchCharacter = Cast<AOverwatch2Character>(TryGetPawnOwner());
	}
	if (OverwatchCharacter == nullptr) return;

	auto Velocity = OverwatchCharacter->GetVelocity();
	//Velocity.Z = 0.f;
	//Speed = Velocity.Size(); // ���� �ӵ�

	// ���� �����̽����� ------------------------------------------------------------- �յ��¿� �̵� ��
	Vertical = FVector::DotProduct(Velocity, OverwatchCharacter->GetActorForwardVector());
	Horizontal = FVector::DotProduct(Velocity, OverwatchCharacter->GetActorRightVector());
	// ���� �����̽����� ------------------------------------------------------------- �յ��¿� �̵� �� ��


	bIsInAir = OverwatchCharacter->GetCharacterMovement()->IsFalling();  // �������� ���̶�� true => ���߿� �ִ�
	bIsCrouched = OverwatchCharacter->bIsCrouched;
	bElimmed = OverwatchCharacter->IsElimmed();
	bAiming = OverwatchCharacter->bIsAiming();

	bRKeyPressed = OverwatchCharacter->GetCombatState() != ECombatState::ECS_Reloading;
}

void UOWAnimInstance::AnimNotify_ReloadFinished()
{
	//if(OverwatchCharacter);
}
