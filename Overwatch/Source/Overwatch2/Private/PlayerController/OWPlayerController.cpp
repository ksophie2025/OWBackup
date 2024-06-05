// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/OWPlayerController.h"
#include "HUD/OWHUD.h"
#include "HUD/MainWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Overwatch2/Overwatch2Character.h"

void AOWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	OWHUD = Cast<AOWHUD>(GetHUD());
}

void AOWPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetHUDTime();
	CheckTimeSync(DeltaTime);
}

// �� �����Ӹ��� timesyncrunningtime�� TimeSyncFrequency�� �����ϸ� ������ �ٸ�? �ٽ� ? ��û ���� (32��°��), ������ �׷� client server delta �ٽ� recalculate�϶�� ���� ����
void AOWPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRPC_RequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;  // ���� �����ֱ�
	}
	// TimeSyncFrequency�� 5�� �ڿ��� ��ũ �Ǿ��־����?
}

void AOWPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	OWHUD = OWHUD == nullptr ? Cast<AOWHUD>(GetHUD()) : OWHUD;
	bool bHUDValid = OWHUD && OWHUD->MainOverlay && OWHUD->MainOverlay->Progressbar_HP && OWHUD->MainOverlay->Text_HP;
	if (bHUDValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDPLZ"))
		const /*int32*/float HealthPercent = Health / MaxHealth;
		OWHUD->MainOverlay->Progressbar_HP->SetPercent(HealthPercent);

		FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		OWHUD->MainOverlay->Text_HP->SetText(FText::FromString(HealthText));
	}
}

void AOWPlayerController::SetHUDScore(float Score)
{
	OWHUD = OWHUD == nullptr ? Cast<AOWHUD>(GetHUD()) : OWHUD;
	bool bHUDValid = OWHUD && OWHUD->MainOverlay && OWHUD->MainOverlay->ScoreAmount;
	if (bHUDValid)
	{	
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		OWHUD->MainOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
}

void AOWPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - /*GetWorld()->GetTimeSeconds()*/GetServerTime());  // GetServerTime()�� �ð��� ������ ��ũ ���߱�
	if (CountdownInt != SecondsLeft)
	{
		SetHUDCountDown(MatchTime - /*GetWorld()->GetTimeSeconds()*/GetServerTime());
	}

	CountdownInt = SecondsLeft;
}



// ������ �ؾ� �� �� : own ���� �ð��̶� Ŭ���̾�Ʈ�� request time�� send back ���ֱ�
// ������ ���� �ð� ���� �� �ְ� 
// Ŭ��RPC �ҷ��� ����
// 
// ������ ��û ������ clientserverdelta ����ϰ� hud�� Ÿ�̸ӿ� ��� ����
// �ִ��� ���� ��ũ ������ϴ� ReceivedPlayer() ���

void AOWPlayerController::ServerRPC_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientRPC_ReportServertTime(TimeOfClientRequest, ServerTimeOfReceipt);  // Ŭ�� �ð���û ���� -> Ŭ�� �ٽ� ���� / ������ ��û �ް� clientRPC �ҷ��� own Time�� send back
}

void AOWPlayerController::ClientRPC_ReportServertTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	// Ŭ�� ���� RPC ������ RTT ��� ����
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;  // Ŭ�� serverRPC ������ �󸶳� �������� �� �� ����
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime); // TimeServerReceivedClientRequest(������ request ����) + rpc�� �����ϴµ� �ɸ� �ð�
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds(); // ���� ���� �ð��� Ŭ�� ���� �ð� �� => Ŭ�� �÷��̾� ��Ʈ�ѷ��� ���� ���۽ð��̶� Ŭ�� ���۽ð� ���� �˰� ��
}


float AOWPlayerController::GetServerTime()
{
	/* if (HasAuthority()) return GetWorld()->GetTimeSeconds();  // ������ ��
	else */return GetWorld()->GetTimeSeconds() + ClientServerDelta;  // Ŭ���� �� , Ŭ��κ��� ���� �����ð� ������ �� ���� ?
}

void AOWPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		// localController�� ServerRPC ���� �� ����
		ServerRPC_RequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AOWPlayerController::SetHUDCountDown(float CountdownTime)
{
	OWHUD = OWHUD == nullptr ? Cast<AOWHUD>(GetHUD()) : OWHUD;
	bool bHUDValid = OWHUD && OWHUD->MainOverlay && OWHUD->MainOverlay->CountDown;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		OWHUD->MainOverlay->CountDown->SetText(FText::FromString(CountdownText));
	}
}

void AOWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AOverwatch2Character* OWCharacter = Cast<AOverwatch2Character>(InPawn);
	if (OWCharacter)
	{
		SetHUDHealth(OWCharacter->GetHealth(), OWCharacter->GetMaxHealth());
	}
}