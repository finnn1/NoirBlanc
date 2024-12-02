// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerUI.h"

#include "Components/TextBlock.h"

void UControllerUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	StartText->SetVisibility(ESlateVisibility::Hidden);
	ShuffleText->SetVisibility(ESlateVisibility::Hidden);
}

void UControllerUI::ShowStartText()
{
	StartText->SetVisibility(ESlateVisibility::Visible);
}

void UControllerUI::HideStartText()
{
	StartText->SetVisibility(ESlateVisibility::Hidden);
}

void UControllerUI::ShowShuffleText()
{
	ShuffleText->SetVisibility(ESlateVisibility::Visible);

	if(BlinkShuffleText)
	{
		PlayAnimation(BlinkShuffleText, 0.0f, 0);	
	}
}

void UControllerUI::HideShuffleText()
{
	ShuffleText->SetVisibility(ESlateVisibility::Hidden);

	if(BlinkShuffleText)
	{
		StopAnimation(BlinkShuffleText);
	}
}
