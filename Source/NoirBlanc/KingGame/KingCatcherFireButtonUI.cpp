// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingCatcherFireButtonUI.h"
#include "KingCatcherPawn.h"
#include "Components/Button.h"

void UKingCatcherFireButtonUI::OnButtonClicked()
{
	// 플레이어 폰을 통해서 서버한테 버튼 눌렀다고 전달
	// 얘를 가지고 있는 플레이어 폰을 불러오자
	AKingCatcherPawn* _CatcherPawn = GetOwningPlayerPawn<AKingCatcherPawn>();
	if (_CatcherPawn)
	{
		_CatcherPawn->HandleButtonClick(Index);
	}
}

void UKingCatcherFireButtonUI::SetButtonSelected(bool bSelected)
{
	if (bSelected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Selected Color hohoho")));
		Button_FireLocation->SetBackgroundColor(FColor::Red);
	}
	else
	{
		Button_FireLocation->SetBackgroundColor(FColor::Silver);
	}
}

void UKingCatcherFireButtonUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_FireLocation->OnClicked.AddDynamic(this, &UKingCatcherFireButtonUI::OnButtonClicked);
}
