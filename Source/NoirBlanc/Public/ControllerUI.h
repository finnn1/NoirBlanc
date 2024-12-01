// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControllerUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UControllerUI : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ShuffleText;

	void ShowShuffleText();
	void HideShuffleText();
};
