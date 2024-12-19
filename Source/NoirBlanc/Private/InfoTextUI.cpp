// Fill out your copyright notice in the Description page of Project Settings.


#include "InfoTextUI.h"
#include "Components/TextBlock.h"

void UInfoTextUI::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if(Controller)
	{
		if(Controller->HasAuthority())
		{
			Text_Color->SetText(FText::FromString(TEXT("블랑[화이트]")));
		}
		else
		{
			Text_Color->SetText(FText::FromString(TEXT("느와르[블랙]")));
		}
	}
}