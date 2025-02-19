// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	TurnStartText->SetVisibility(ESlateVisibility::Hidden);
	EnmTurnStartText->SetVisibility(ESlateVisibility::Hidden);
	WinText->SetVisibility(ESlateVisibility::Hidden);
	LoseText->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerUI::SetNoirTurnText()
{
	TurnStartText->SetColorAndOpacity(FColor::Black);
	FSlateFontInfo NoirFontInfo = TurnStartText->GetFont();
	NoirFontInfo.OutlineSettings.OutlineColor = FColor::White;
	TurnStartText->SetFont(NoirFontInfo);

	EnmTurnStartText->SetColorAndOpacity(FColor::White);
	FSlateFontInfo BlanFontInfo = EnmTurnStartText->GetFont();
	BlanFontInfo.OutlineSettings.OutlineColor = FColor::Black;
	EnmTurnStartText->SetFont(BlanFontInfo);
}

void UPlayerUI::SetBlanTurnText()
{
	TurnStartText->SetColorAndOpacity(FColor::White);
	FSlateFontInfo BlanFontInfo = TurnStartText->GetFont();
	BlanFontInfo.OutlineSettings.OutlineColor = FColor::Black;
	TurnStartText->SetFont(BlanFontInfo);

	EnmTurnStartText->SetColorAndOpacity(FColor::Black);
	FSlateFontInfo NoirFontInfo = EnmTurnStartText->GetFont();
	NoirFontInfo.OutlineSettings.OutlineColor = FColor::White;
	EnmTurnStartText->SetFont(NoirFontInfo);
}

void UPlayerUI::ShowTurnStart()
{
	TurnStartText->SetVisibility(ESlateVisibility::Visible);
	//GetWorld()->GetTimerManager().SetTimer(StartTimeHandler, this, &UPlayerUI::HideTurnStart, 1.f, false);
	if(ShowMyTurn)
	{
		PlayAnimation(ShowMyTurn);
	}
}

void UPlayerUI::HideTurnStart()
{
	//GetWorld()->GetTimerManager().ClearTimer(StartTimeHandler);
	if(HideMyTurn)
	{
		PlayAnimation(HideMyTurn);

		FTimerHandle TimerHandle;
		float AnimTime = ShowEnemyTurn->GetEndTime();
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &TimerHandle]()
		{
			TurnStartText->SetVisibility(ESlateVisibility::Hidden);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}, AnimTime+1.f, false);
	}
}

void UPlayerUI::ShowEnmTurnStart()
{
	EnmTurnStartText->SetVisibility(ESlateVisibility::Visible);
	//GetWorld()->GetTimerManager().SetTimer(EnmStartTimeHandler, this, &UPlayerUI::HideEnmTurnStart, 1.f, false);
	if(ShowEnemyTurn)
	{
		PlayAnimation(ShowEnemyTurn);
	}
}

void UPlayerUI::HideEnmTurnStart()
{
	//GetWorld()->GetTimerManager().ClearTimer(EnmStartTimeHandler);
	if(HideEnemyTurn)
	{
		PlayAnimation(HideEnemyTurn);
		
		FTimerHandle TimerHandle;
		float AnimTime = ShowEnemyTurn->GetEndTime();
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &TimerHandle]()
		{
			EnmTurnStartText->SetVisibility(ESlateVisibility::Hidden);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}, AnimTime, false);
	}
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
