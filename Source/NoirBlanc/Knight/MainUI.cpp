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
	
	Text_MyDistance = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_MyDistance")));
	Text_EnemyDistance = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_EnemyDistance")));
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//UpdateTimerText(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->GameTimeLeft);
}

void UMainUI::UpdateTimerText(int32 time)
{
	Text_Timer->SetText(FText::FromString(FString::FromInt(time)));
}

void UMainUI::UpdateMyDistance(float distance)
{
	Text_MyDistance->SetText(FText::FromString(FString::FromInt(floor(distance))));
}

void UMainUI::UpdateEnemyDistance(float distance)
{
	Text_EnemyDistance->SetText(FText::FromString(FString::FromInt(floor(distance))));
}





