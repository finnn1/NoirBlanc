// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
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
	UTextBlock* Text_Blanc;
	UTextBlock* Text_Noir;
	
	UPROPERTY()
	UCanvasPanel* Turn_Blanc;
	UCanvasPanel* Turn_Noir;
public:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_Blanc;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_Noir;
	
	FTimerHandle Handle;

public:
	void ShowTurn(EPieceColor color);
	void HideBlanc();
};
