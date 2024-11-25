// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/TextBlock.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	Text_Timer = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Timer")));
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainUI::UpdateTimerText(float time)
{
	int32 sec = floor(time);
	if(sec < 0)
	{
		sec = 0;
	}
	Text_Timer->SetText(FText::FromString(FString::FromInt(sec)));
}

