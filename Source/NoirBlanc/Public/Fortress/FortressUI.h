// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "NoirBlanc/Knight/CountDownUI.h"
#include "NoirBlanc/Knight/TurnUI.h"
#include "NoirBlanc/Knight/WaitingUI.h"
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

public:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	// Waiting UI
public:
	UPROPERTY(meta = (BindWidget))
	UWaitingUI* WBP_WaitingUI;
	
	// Start UI
public:
	UPROPERTY(meta = (BindWidget))
	UCountDownUI* WBP_CountDownUI; 

	FTimerHandle CountdownTimer;
	
	int32 CountdownTime;
	void UpdateCountdown();

	// Main UI
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1Info;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2Info;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Player1pg;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Player2pg;

	ACannon* playerCannon;
	// Cannon* Player2;

	// current turn Cannon
	EPieceColor playerPieceColor;
	
	UPROPERTY(BlueprintReadOnly)
	float Player1Percentage;
	
	UPROPERTY(BlueprintReadOnly)
	float Player2Percentage;
	
	// when the cannon's health change, change the progress bar
	UFUNCTION(BlueprintCallable)
	void ApplyDamageHPBar(ACannon* damagedCannon, ACannon* player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTurnUI> TurnUIFactory;
	
	UPROPERTY(meta=(BindWidget))
	UTurnUI* turnUI;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UHorizontalBox* ImageHorizontalBox;

	UPROPERTY(meta=(BindWidgetOptional))
	TArray<UImage*> ArrowImages;
	
	// End UI
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFinishUI> FinishUIFactory;

	UPROPERTY(meta = (BindWidget))
	UFinishUI* FinishUI;
	
	void GameOver(int32 index);

public:
	UFUNCTION(BlueprintCallable)
	void SetWindBar(int32 Strength);
};


