// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OWGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AOWGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AOWGameMode();

	virtual void PlayerEliminated(class AOverwatch2Character* ElimmedCharacter, class AOWPlayerController* VictimController, class AOWPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);
};
