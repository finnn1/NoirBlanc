// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Components/TextBlock.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	TurnStartText->SetVisibility(ESlateVisibility::Hidden);
	TurnEndText->SetVisibility(ESlateVisibility::Hidden);
	EnmTurnStartText->SetVisibility(ESlateVisibility::Hidden);
	EnmTurnEndText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerUI::ShowTurnStart()
{
	TurnStartText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(StartTimeHandler, this, &UPlayerUI::HideTurnStart, 1.f, false);
}

void UPlayerUI::HideTurnStart()
{
	TurnStartText->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(StartTimeHandler);
}

void UPlayerUI::ShowTurnEnd()
{
	TurnEndText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(EndTimeHandler, this, &UPlayerUI::HideTurnEnd, 1.f, false);
}

void UPlayerUI::HideTurnEnd()
{
	TurnEndText->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(EndTimeHandler);
}

void UPlayerUI::ShowEnmTurnStart()
{
	EnmTurnStartText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(EnmStartTimeHandler, this, &UPlayerUI::HideEnmTurnStart, 1.f, false);
}

void UPlayerUI::HideEnmTurnStart()
{
	EnmTurnStartText->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(EnmStartTimeHandler);
}

void UPlayerUI::ShowEnmTurnEnd()
{
	EnmTurnEndText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(EnmEndTimeHandler, this, &UPlayerUI::HideEnmTurnEnd, 1.f, false);
}

void UPlayerUI::HideEnmTurnEnd()
{
	EnmTurnEndText->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(EnmEndTimeHandler);
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
	UE_LOG(LogTemp, Warning, TEXT("UI score is %f"), score);
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
