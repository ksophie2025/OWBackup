// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/OWGameState.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/OWPlayerState.h"

void AOWGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOWGameState, TopScoringPlayers);

}

void AOWGameState::UpdateTopScore(AOWPlayerState* ScoringPlayer)
{
	// 리드 뺏기면 배열에서 없애기?
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		// score 추적해서 현재 top 보다 높으면 배열 업데이트
		TopScore = ScoringPlayer->GetScore();
	}
	// 동점
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer); // 이미 배열에 있는 플레이어면 추가 x 
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}
