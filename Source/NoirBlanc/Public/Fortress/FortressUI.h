// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Blueprint/UserWidget.h"
#include "FortressUI.generated.h"


class ACannon;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class NOIRBLANC_API UFortressUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// Start UI
public:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* text_GameStart;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* text_Countdown;

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	// FTimerHandle CountdownTimer;
	
	// int32 CountdownTime;
	// void UpdateCountdown();

	// Main UI
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Player1pg;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Player2pg;

	ACannon* playerCannon;
	// Cannon* Player2;

	UPROPERTY(BlueprintReadOnly)
	float Player1Percentage;
	
	UPROPERTY(BlueprintReadOnly)
	float Player2Percentage;
	
	// when the cannon's health change, change the progress bar
	UFUNCTION(BlueprintCallable)
	void ApplyDamageHPBar(ACannon* damagedCannon, ACannon* player);

	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* horizontalBox_Turn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* text_Turn;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* windForcePlus;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* windForceMinus;
	// End UI
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* text_Winner;

	void GameOver(int32 index);

	// Etc. 
public:
	void SetTurnWidgetVisible(); 
	
	void SetTurnWidgetHidden();

public:
	UFUNCTION(BlueprintCallable)
	void SetWindBar(float percent);
};


