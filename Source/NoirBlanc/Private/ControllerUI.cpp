// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerUI.h"

#include "Components/TextBlock.h"

void UControllerUI::NativeConstruct()
{
	Super::NativeConstruct();

	//ShuffleText->SetVisibility(ESlateVisibility::Hidden);
}

void UControllerUI::ShowShuffleText()
{
	ShuffleText->SetVisibility(ESlateVisibility::Visible);
}

void UControllerUI::HideShuffleText()
{
	ShuffleText->SetVisibility(ESlateVisibility::Hidden);
}
