// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	TurnStartText->SetVisibility(ESlateVisibility::Hidden);
	EnmTurnStartText->SetVisibility(ESlateVisibility::Hidden);
	WinText->SetVisibility(ESlateVisibility::Hidden);
	LoseText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerUI::ShowTurnStart()
{
	TurnStartText->SetVisibility(ESlateVisibility::Visible);
	//GetWorld()->GetTimerManager().SetTimer(StartTimeHandler, this, &UPlayerUI::HideTurnStart, 1.f, false);
}

void UPlayerUI::HideTurnStart()
{
	TurnStartText->SetVisibility(ESlateVisibility::Hidden);
	//GetWorld()->GetTimerManager().ClearTimer(StartTimeHandler);
}

void UPlayerUI::ShowEnmTurnStart()
{
	EnmTurnStartText->SetVisibility(ESlateVisibility::Visible);
	//GetWorld()->GetTimerManager().SetTimer(EnmStartTimeHandler, this, &UPlayerUI::HideEnmTurnStart, 1.f, false);
}

void UPlayerUI::HideEnmTurnStart()
{
	EnmTurnStartText->SetVisibility(ESlateVisibility::Hidden);
	//GetWorld()->GetTimerManager().ClearTimer(EnmStartTimeHandler);
}


void UPlayerUI::IncreaseScore()
{
	if(MyPlayerScoreText)
	{
		FString ScoreStr = MyPlayerScoreText->GetText().ToString();
		int32 IntVariable = FCString::Atoi(*ScoreStr);
		
		IntVariable++;
		MyPlayerScoreText->SetText(FText::AsNumber(IntVariable));
	}
}

void UPlayerUI::IncreaseEnemyScore()
{
	if(EnemyPlayerScoreText)
	{
		FString ScoreStr = EnemyPlayerScoreText->GetText().ToString();
		int32 IntVariable = FCString::Atoi(*ScoreStr);
		
		IntVariable++;
		EnemyPlayerScoreText->SetText(FText::AsNumber(IntVariable));
	}
}

void UPlayerUI::SetMyScore(float score)
{
	if(MyPlayerScoreText)
	{
		MyPlayerScoreText->SetText(FText::AsNumber(score));
	}
}

void UPlayerUI::SetEnemyScore(float score)
{
	if(EnemyPlayerScoreText)
	{
		EnemyPlayerScoreText->SetText(FText::AsNumber(score));
	}
}

void UPlayerUI::ShowWinPlayer()
{
	WinText->SetVisibility(ESlateVisibility::Visible);	
}

void UPlayerUI::ShowLosePlayer()
{
	LoseText->SetVisibility(ESlateVisibility::Visible);
}