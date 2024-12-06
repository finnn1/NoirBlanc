// Fill out your copyright notice in the Description page of Project Settings.


#include "NoirBlanc/KingGame/TempLobbyUI.h"

#include "NoirBlancGameInstance.h"
#include "TempLobbySessionItem.h"
#include "Components/Button.h"
#include "Components/ComboBoxKey.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UTempLobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UNoirBlancGameInstance>(GetGameInstance());

	Btn_GoToCreateSession->OnClicked.AddDynamic(this, &UTempLobbyUI::UTempLobbyUI::GoToCreateSession);
	Btn_FindSession->OnClicked.AddDynamic(this, &UTempLobbyUI::UTempLobbyUI::FindSession);

	Btn_CreateSession->OnClicked.AddDynamic(this, &UTempLobbyUI::CreateSession);
	Btn_GoBackToSessionList->OnClicked.AddDynamic(this, &UTempLobbyUI::GoBackSessionList);

	ComboBox_GameList->OnGenerateContentWidget.BindDynamic(this, &UTempLobbyUI::OnGenerateContentWidget);
	ComboBox_GameList->OnGenerateItemWidget.BindDynamic(this, &UTempLobbyUI::OnGenerateItemWidget);

	ComboBox_GameList->AddOption(FName(TEXT("King")));
	ComboBox_GameList->AddOption(FName(TEXT("Queen")));
	ComboBox_GameList->AddOption(FName(TEXT("Rook")));
	ComboBox_GameList->AddOption(FName(TEXT("Bishop")));
	ComboBox_GameList->AddOption(FName(TEXT("Knight")));
	ComboBox_GameList->AddOption(FName(TEXT("Pawn")));
	ComboBox_GameList->AddOption(FName(TEXT("Chess")));

	ComboBox_GameList->SetSelectedOption(FName(TEXT("King")));
	if(GameInstance)
	{
		GameInstance->OnAddSession.BindUObject(this, &UTempLobbyUI::OnAddSession);
		GameInstance->OnFindComplete.BindUObject(this, &UTempLobbyUI::OnFindComplete);
	}
}

void UTempLobbyUI::GoToCreateSession()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UTempLobbyUI::FindSession()
{
	// 게임 인스턴스에서 처리할 예정.
	GameInstance->FindOtherSession();
}

UWidget* UTempLobbyUI::OnGenerateContentWidget(FName ItemText)
{
	UTextBlock* NewTextBlock = NewObject<UTextBlock>();
	NewTextBlock->SetText(FText::FromName(ItemText));
	NewTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));

	return NewTextBlock;
}

UWidget* UTempLobbyUI::OnGenerateItemWidget(FName ItemText)
{
	UTextBlock* NewTextBlock = NewObject<UTextBlock>();
	NewTextBlock->SetText(FText::FromName(ItemText));
	NewTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));

	return NewTextBlock;
}

void UTempLobbyUI::CreateSession()
{
	// 게임 인스턴스에서 처리할 예정.

	// edit_DisplayName 에 값이 없으면 함수 나가자.
	if (TextBox_SessionName->GetText().IsEmpty()) return;

	// 세션 생성
	FString DisplayName = TextBox_SessionName->GetText().ToString();
	FName SelectedGame = ComboBox_GameList->GetSelectedOption();
	GameInstance->CreateSession(DisplayName, SelectedGame);
}

void UTempLobbyUI::GoBackSessionList()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UTempLobbyUI::OnAddSession(int32 Index, FString Info)
{
	UTempLobbySessionItem* Item = CreateWidget<UTempLobbySessionItem>(GetWorld(), SessionItemClass);
	if (Item)
	{
		Scroll_Sessions->AddChild(Item);
		Item->SetInfo(Index, Info);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session item widget class is not specified!!"));
	}
}

void UTempLobbyUI::OnFindComplete(bool bIsComplete)
{
	// 검색 버튼 활성/비활성
	Btn_FindSession->SetIsEnabled(bIsComplete);
}
