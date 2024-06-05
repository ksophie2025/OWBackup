// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH2_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Progressbar_HP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_HP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountDown;
};
