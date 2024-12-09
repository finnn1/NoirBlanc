// Fill out your copyright notice in the Description page of Project Settings.

#include "WaitingOtherPlayerUI.h"
#include "Components/TextBlock.h"

void UWaitingOtherPlayerUI::SetText(const FText& NewText)
{
	if (MainText)
	{
		MainText->SetText(NewText);
	}
}
