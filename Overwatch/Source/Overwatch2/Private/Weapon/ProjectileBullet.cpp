// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "OWComponents/CombatComponent.h"
// #include "Overwatch2/Overwatch2Character.h"
// #include "OWComponents/CombatComponent.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuse, const FHitResult& Hit)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->Controller;
		if (OwnerController)
		{
			int32 FinalDamage = Damage;

			UCombatComponent* CombatComponent = OwnerCharacter->FindComponentByClass<UCombatComponent>();
			if (CombatComponent && CombatComponent->bAiming)
			{
				float ChargeRatio = FMath::Min(CombatComponent->ChargeTime / CombatComponent->MaxChargeTime, 1.0f);
				FinalDamage = FMath::Lerp(CombatComponent->BaseDamage, CombatComponent->ChargedDamage, ChargeRatio);
			}

			UGameplayStatics::ApplyDamage(OtherActor, FinalDamage, OwnerController, this, UDamageType::StaticClass());
			UE_LOG(LogTemp, Warning, TEXT("%d"), FinalDamage);	
		}
	}
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpuse, Hit);  // destroy() 있어서 늦게 호출 
}
