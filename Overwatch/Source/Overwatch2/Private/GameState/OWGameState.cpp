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
	// ���� ����� �迭���� ���ֱ�?
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		// score �����ؼ� ���� top ���� ������ �迭 ������Ʈ
		TopScore = ScoringPlayer->GetScore();
	}
	// ����
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer); // �̹� �迭�� �ִ� �÷��̾�� �߰� x 
	}
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}
