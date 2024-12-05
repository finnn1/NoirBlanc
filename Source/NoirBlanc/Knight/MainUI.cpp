// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/TextBlock.h"
#include "NoirBlanc/Knight/GameStateBase_Knight.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text_Timer = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Timer")));
	
	Text_ServerDistance = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_ServerDistance")));
	Text_ClientDistance = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_ClientDistance")));
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainUI::UpdateTimerText(int32 time)
{
	Text_Timer->SetText(FText::FromString(FString::FromInt(time)));
}

void UMainUI::UpdateServerDistance(float distance)
{
	Text_ServerDistance->SetText(FText::FromString(FString::FromInt(floor(distance))));
}

void UMainUI::UpdateClientDistance(float distance)
{
	Text_ClientDistance->SetText(FText::FromString(FString::FromInt(floor(distance))));
}





