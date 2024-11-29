// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUI.h"

void UStartUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Ready->OnClicked.AddDynamic(this, &UStartUI::OnClickReady);
}

void UStartUI::OnClickReady()
{
	if(GetOwningPlayerPawn()->HasAuthority())
	{
		Txt_ServerReady->SetText(FText::FromString("O"));
	}
	else
	{
		Txt_ClientReady->SetText(FText::FromString("O"));
	}
}
