// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishUI.h"

void UFinishUI::NativeConstruct()
{
	Super::NativeConstruct();
	Txt_Winner =  Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Winner")));
}

void UFinishUI::UpdateWinnerText(FText winner)
{
	Txt_Winner->SetText(winner);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *winner.ToString());
	Txt_Winner->InvalidateLayoutAndVolatility();
}
