// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/OWGameMode.h"
#include "Overwatch2/Overwatch2Character.h"
#include "PlayerController/OWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerState/OWPlayerState.h"
#include "GameState/OWGameState.h"

AOWGameMode::AOWGameMode()
{
	// bDelayedStart = true;
}

void AOWGameMode::PlayerEliminated(AOverwatch2Character* ElimmedCharacter, AOWPlayerController* VictimController, AOWPlayerController* AttackerController)
{
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr) return;
//	AOWPlayerState* AttackerPlayerState = AttackerPlayerState ? Cast<AOWPlayerState>(AttackerController->PlayerState) : nullptr;
//	AOWPlayerState* VictimPlayerState = VictimPlayerState ? Cast<AOWPlayerState>(VictimController->PlayerState) : nullptr;
	AOWPlayerState* AttackerPlayerState = nullptr;
	if (AttackerController->PlayerState)
	{
		AttackerPlayerState = Cast<AOWPlayerState>(AttackerController->PlayerState);
	}
	AOWPlayerState* VictimPlayerState = nullptr;
	if (VictimController->PlayerState)
	{
		VictimPlayerState = Cast<AOWPlayerState>(VictimController->PlayerState);
	}

	AOWGameState* OWGameState = GetGameState<AOWGameState>();

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && OWGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
		OWGameState->UpdateTopScore(AttackerPlayerState);
	}
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AOWGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedCharacter)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
