// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/OWPlayerState.h"
#include "Overwatch2/Overwatch2Character.h"
#include "PlayerController/OWPlayerController.h"
#include "Net/UnrealNetwork.h"

void AOWPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore()+ ScoreAmount);
	Character = Character == nullptr ? Cast<AOverwatch2Character>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AOWPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void AOWPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<AOverwatch2Character>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AOWPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}


