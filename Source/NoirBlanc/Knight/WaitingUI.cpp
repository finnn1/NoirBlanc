// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingUI.h"
#include "TimerManager.h"

void UWaitingUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWaitingUI::DestroyWaitingUI()
{
	Txt_Waiting->SetText(FText::FromString(TEXT("시작!")));
	
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UWaitingUI::RemoveUI, 1, false);
	
}

void UWaitingUI::RemoveUI()
{
	RemoveFromParent();
}
