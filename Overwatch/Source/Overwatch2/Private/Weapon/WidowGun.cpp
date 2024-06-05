 // Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WidowGun.h"
#include "Engine/SkeletalMeshSocket.h" 
#include "Overwatch2/Overwatch2Character.h"
#include "Kismet/GameplayStatics.h"

void AWidowGun::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("WidowMuzzleFlash");
	if (MuzzleFlashSocket)
	{
		//UE_LOG(LogTemp, Warning, TEXT("1234123412342134234"));
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = (HitTarget - Start) * 1.25f;
		//if (InstigatorPawn == nullptr)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("DAFASDFSDFSAF"));
		//}
		FHitResult FireHit;
		UWorld* World = GetWorld();
		if (World)
		{
			World->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
		}
		if (FireHit.bBlockingHit)
		{
			AOverwatch2Character* OWCharacter = Cast<AOverwatch2Character>(FireHit.GetActor());
			if (OWCharacter)
			{
				if (HasAuthority())
				{
					UGameplayStatics::ApplyDamage(OWCharacter, Damage, InstigatorController, this, UDamageType::StaticClass());
				}
			}
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
			}
		}
	}
}
