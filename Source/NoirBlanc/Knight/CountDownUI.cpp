// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownUI.h"

void UCountDownUI::NativeConstruct()
{
	Super::NativeConstruct();

	Txt_Count = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Count")));
}
