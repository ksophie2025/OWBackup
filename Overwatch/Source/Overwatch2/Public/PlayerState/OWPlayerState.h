// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OWPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AOWPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void OnRep_Score() override;
	void AddToScore(float ScoreAmount);

private:
	class AOverwatch2Character* Character;
	class AOWPlayerController* Controller;
};
