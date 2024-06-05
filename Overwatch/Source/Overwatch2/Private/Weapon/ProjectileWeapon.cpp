// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"
#include "Weapon/Projectile.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/SkeletalMeshSocket.h>

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if (!HasAuthority()) return;

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	//	UWorld* World = GetWorld();
	if (MuzzleFlashSocket/* && World*/)
	{
		//UE_LOG(LogTemp, Warning, TEXT("1234123412342134234"));
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
		//if (InstigatorPawn == nullptr)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("DAFASDFSDFSAF"));
		//}
		if (ProjectileClass && InstigatorPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
			}
		}
	}
}
