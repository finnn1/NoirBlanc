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
	UTextBlock* EnmTurnStartText;

	// Noir로 대체
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MyPlayerScoreText;

	// Blan으로 대체
	UPROPERTY(meta=(BindWidget))
	UTextBlock* EnemyPlayerScoreText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WinText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* LoseText;

	FTimerHandle StartTimeHandler;
	FTimerHandle EnmStartTimeHandler;

public:
	virtual void NativeConstruct() override;

	// 턴 시작 문구
	void ShowTurnStart();
	void HideTurnStart();

	// 적 턴 시작 문구
	void ShowEnmTurnStart();
	void HideEnmTurnStart();

	// 점수 증가
	void IncreaseScore();
	void IncreaseEnemyScore();

	void SetMyScore(float score);
	void SetEnemyScore(float score);

	// 게임 끝
	void ShowWinPlayer();
	void ShowLosePlayer();
};
