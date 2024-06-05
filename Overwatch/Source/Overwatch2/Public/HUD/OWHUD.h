// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OWHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API AOWHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> MainOverlayClass;

	class UMainWidget* MainOverlay;

protected:
	virtual void BeginPlay() override;
	void AddMainOverlay();
};
