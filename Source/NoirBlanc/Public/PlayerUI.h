// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class NOIRBLANC_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TurnStartText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TurnEndText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* EnmTurnStartText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* EnmTurnEndText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MyPlayerScoreText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* EnemyPlayerScoreText;

	FTimerHandle StartTimeHandler;
	FTimerHandle EndTimeHandler;

	FTimerHandle EnmStartTimeHandler;
	FTimerHandle EnmEndTimeHandler;

public:
	virtual void NativeConstruct() override;

	// 턴 시작 문구
	void ShowTurnStart();
	void HideTurnStart();

	// 턴 끝 문구
	void ShowTurnEnd();
	void HideTurnEnd();

	// 적 턴 시작 문구
	void ShowEnmTurnStart();
	void HideEnmTurnStart();
	
	// 적 턴 끝 문구
	void ShowEnmTurnEnd();
	void HideEnmTurnEnd();

	// 점수 증가
	void IncreaseScore();
	void IncreaseEnemyScore();

	void SetMyScore(float score);
	void SetEnemyScore(float score);
};
