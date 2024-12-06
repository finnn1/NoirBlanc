// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/TempLobbySessionItem.h"

#include "NoirBlancGameInstance.h"
//#include "TempNoirBlancGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTempLobbySessionItem::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Join->OnClicked.AddDynamic(this, &UTempLobbySessionItem::JoinSession);
}

void UTempLobbySessionItem::JoinSession()
{
	UNoirBlancGameInstance* GameInstance = Cast<UNoirBlancGameInstance>(GetGameInstance());
	GameInstance->JoinOtherSession(SelectIndex);
}

void UTempLobbySessionItem::SetInfo(int32 Index, FString Info)
{
	SelectIndex = Index;
	Text_SessionName->SetText(FText::FromString(Info));
}
