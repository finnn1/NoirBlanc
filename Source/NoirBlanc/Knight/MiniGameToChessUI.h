// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PieceTypes.h"
#include "MiniGameToChessUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UMiniGameToChessUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_LoseTeam;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_LosePiece;
	
	void UpdateMiniGameToChessUI(EPieceColor LoserColor, EPieceType DeletePieceType);

private:
	void DestroyUI();
};