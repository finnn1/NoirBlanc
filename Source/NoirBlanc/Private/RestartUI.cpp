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
	GameInstance->ServerRPC_InitInstance();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			AChessPlayerController* Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
			Controller->ServerRPC_LevelTravel(TEXT("/Game/Level/Lv_ChessBoard?listen"));
		}, 1.0f, false);

}

void URestartUI::OnEndClicked()
{
	AChessPlayerController* Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->ServerRPC_Quit();
}