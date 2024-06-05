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

// 매 프레임마다 timesyncrunningtime이 TimeSyncFrequency에 도달하면 서버에 다른? 다시 ? 요청 보냄 (32번째줄), 서버는 그럼 client server delta 다시 recalculate하라고 응답 보냄
void AOWPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRPC_RequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;  // 리셋 시켜주기
	}
	// TimeSyncFrequency인 5초 뒤에는 싱크 되어있어야함?
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
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - /*GetWorld()->GetTimeSeconds()*/GetServerTime());  // GetServerTime()로 시간을 서버랑 싱크 맞추기
	if (CountdownInt != SecondsLeft)
	{
		SetHUDCountDown(MatchTime - /*GetWorld()->GetTimeSeconds()*/GetServerTime());
	}

	CountdownInt = SecondsLeft;
}



// 서버가 해야 할 거 : own 서버 시간이랑 클라이언트의 request time을 send back 해주기
// 서버의 현재 시간 얻을 수 있고 
// 클라RPC 불러서 전달
// 
// 서버에 요청 보내고 clientserverdelta 계산하고 hud의 타이머에 사용 가능
// 최대한 빨리 싱크 맞춰야하니 ReceivedPlayer() 사용

void AOWPlayerController::ServerRPC_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientRPC_ReportServertTime(TimeOfClientRequest, ServerTimeOfReceipt);  // 클라 시간요청 받음 -> 클라에 다시 전달 / 서버는 요청 받고 clientRPC 불러서 own Time을 send back
}

void AOWPlayerController::ClientRPC_ReportServertTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	// 클라가 위의 RPC 받으면 RTT 계산 가능
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;  // 클라가 serverRPC 보내고 얼마나 지났는지 알 수 있음
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime); // TimeServerReceivedClientRequest(서버가 request 받은) + rpc가 도착하는데 걸린 시간
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds(); // 서버 현재 시간과 클라 현재 시간 차 => 클라 플레이어 컨트롤러가 서버 시작시간이랑 클라 시작시간 차이 알게 됨
}


float AOWPlayerController::GetServerTime()
{
	/* if (HasAuthority()) return GetWorld()->GetTimeSeconds();  // 서버일 때
	else */return GetWorld()->GetTimeSeconds() + ClientServerDelta;  // 클라일 때 , 클라로부터 현재 서버시간 가져올 수 있음 ?
}

void AOWPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		// localController면 ServerRPC 보낼 수 있음
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