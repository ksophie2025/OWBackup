// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AOWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDCountDown(float CountdownTime);
	virtual void OnPossess(APawn* InPawn);
	virtual void Tick(float DeltaTime) override;

	
	virtual float GetServerTime(); // 서버 현재 시간 반환
	virtual void ReceivedPlayer() override;  // 서버로부터 시간 가장 빨리 가져올 수 있음, 요청 보내는 곳 / 서버 시계랑 가능한 빨리 싱크 맞추는 함수

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();

	/**
	* 서버랑 클라 시간 싱크 맞추기
	*/
	// 현재 서버 시간 요청, 요청 전송된 클라 시간 전달
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestServerTime(float TimeOfClientRequest);

	// 클라이언트에 다시 보낼 서버RPC, ServerRPC_RequestServerTime에 대한 현재 서버 시간을 클라에 응답 
	UFUNCTION(Client, Reliable)
	void ClientRPC_ReportServertTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f;  // 서버 현재 시간과 클라 현재 시간 차

	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;  // 5초마다 서버 시간으로 싱크 , last sync 타임부터 얼마나 지났는지도 체크 필요 

	float TimeSyncRunningTime = 0.f;  // last sync 타임부터 얼마나 지났는지도 체크 필요, tick에서 업데이트하고 싱크 맞추고 다시 reset
	void CheckTimeSync(float DeltaTime);

private:
	class AOWHUD* OWHUD;

	float MatchTime = 120.f;
	uint32 CountdownInt = 0;
};
