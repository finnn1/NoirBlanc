// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PieceTypes.h"
#include "Blueprint/UserWidget.h"
#include "BattleUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class NOIRBLANC_API UBattleUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Piece;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Game;

	void UpdateBattleUI(EPieceType GamePiece);
};
