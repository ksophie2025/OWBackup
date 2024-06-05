// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OWHUD.h"
#include "HUD/MainWidget.h"

void AOWHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AOWHUD::BeginPlay()
{
	Super::BeginPlay();

	AddMainOverlay();
}

void AOWHUD::AddMainOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && MainOverlayClass)
	{
		MainOverlay = CreateWidget<UMainWidget>(PlayerController, MainOverlayClass);
		MainOverlay->AddToViewport();

	}
}
