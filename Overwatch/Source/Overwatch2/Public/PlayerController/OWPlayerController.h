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

	
	virtual float GetServerTime(); // ���� ���� �ð� ��ȯ
	virtual void ReceivedPlayer() override;  // �����κ��� �ð� ���� ���� ������ �� ����, ��û ������ �� / ���� �ð�� ������ ���� ��ũ ���ߴ� �Լ�

protected:
	virtual void BeginPlay() override;
	void SetHUDTime();

	/**
	* ������ Ŭ�� �ð� ��ũ ���߱�
	*/
	// ���� ���� �ð� ��û, ��û ���۵� Ŭ�� �ð� ����
	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestServerTime(float TimeOfClientRequest);

	// Ŭ���̾�Ʈ�� �ٽ� ���� ����RPC, ServerRPC_RequestServerTime�� ���� ���� ���� �ð��� Ŭ�� ���� 
	UFUNCTION(Client, Reliable)
	void ClientRPC_ReportServertTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f;  // ���� ���� �ð��� Ŭ�� ���� �ð� ��

	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;  // 5�ʸ��� ���� �ð����� ��ũ , last sync Ÿ�Ӻ��� �󸶳� ���������� üũ �ʿ� 

	float TimeSyncRunningTime = 0.f;  // last sync Ÿ�Ӻ��� �󸶳� ���������� üũ �ʿ�, tick���� ������Ʈ�ϰ� ��ũ ���߰� �ٽ� reset
	void CheckTimeSync(float DeltaTime);

private:
	class AOWHUD* OWHUD;

	float MatchTime = 120.f;
	uint32 CountdownInt = 0;
};
