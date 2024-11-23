// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Components/TextBlock.h"

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
