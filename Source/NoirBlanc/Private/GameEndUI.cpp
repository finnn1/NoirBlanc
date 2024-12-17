// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndUI.h"

#include "Components/TextBlock.h"

void UGameEndUI::NativeConstruct()
{
	Super::NativeConstruct();
	Txt_Winner =  Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Winner")));
}

void UGameEndUI::UpdateWinnerText(FText winner)
{
	Txt_Winner->SetText(winner);
}