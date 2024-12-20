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
	// 체크메이트 뜬다
	// 킹 두개가 부딫힌다
	
	// 둘 중 진말이 사라진다
	if(winner.EqualTo(FText::FromString(TEXT("블랑"))))
	{
		
	}
	else
	{
		
	}
	
	// 땡땡 승리
	Txt_Winner->SetText(winner);
}