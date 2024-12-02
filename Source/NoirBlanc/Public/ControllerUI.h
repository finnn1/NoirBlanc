// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControllerUI.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class NOIRBLANC_API UControllerUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* StartText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ShuffleText;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* BlinkShuffleText;

	void ShowStartText();
	void HideStartText();

	void ShowShuffleText();
	void HideShuffleText();
};
