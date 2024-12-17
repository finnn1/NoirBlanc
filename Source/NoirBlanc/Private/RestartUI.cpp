// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartUI.h"

#include "ChessPlayerController.h"
#include "NoirBlancGameInstance.h"
#include "Components/Button.h"

void URestartUI::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Restart->OnClicked.AddDynamic(this, &URestartUI::OnRestartClicked);
	Btn_End->OnClicked.AddDynamic(this, &URestartUI::OnEndClicked);
}

void URestartUI::OnRestartClicked()
{
	UNoirBlancGameInstance* GameInstance = Cast<UNoirBlancGameInstance>(GetGameInstance());
	GameInstance->Init();
	AChessPlayerController* Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->ServerRPC_LevelTravel(TEXT("/Game/Level/Lv_ChessBoard?listen"));
}

void URestartUI::OnEndClicked()
{
	AChessPlayerController* Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->ServerRPC_Quit();
}