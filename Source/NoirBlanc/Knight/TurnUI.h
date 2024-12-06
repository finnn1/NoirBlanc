// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TurnUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UTurnUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UImage* Bg;
	UTextBlock* Text_Turn;
	UPROPERTY()
	UCanvasPanel* Turn;

	
	FTimerHandle Handle;

public:
	void ShowTurn();
	void HideTurn();
};
