// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OWGameState.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AOWGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateTopScore(class AOWPlayerState* ScoringPlayer);

	UPROPERTY(Replicated)
	TArray<class AOWPlayerState*> TopScoringPlayers;

private:
	float TopScore = 0.f;
};
