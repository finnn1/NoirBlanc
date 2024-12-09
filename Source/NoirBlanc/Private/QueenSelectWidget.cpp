// Fill out your copyright notice in the Description page of Project Settings.


#include "QueenSelectWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UQueenSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

// FName UQueenSelectWidget::ChooseLevel()
// {
// 	UGameplayStatics::SetGamePaused(GetWorld(), true);
// }

// void AYourClass::ShowPauseMenu()
// {
// 	// 게임 일시 정지
// 	
//
// 	// 입력 모드를 UI 전용으로 설정
//
//
// 		// 입력 모드 변경
// 		FInputModeUIOnly InputMode;
// 		InputMode.SetWidgetToFocus(PauseMenu->TakeWidget());
// 		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
// 		PlayerController->SetInputMode(InputMode);
//
// 		// 마우스 커서 표시
// 		PlayerController->bShowMouseCursor = true;
// 	}
// }
//
// void AYourClass::HidePauseMenu()
// {
// 	// 게임 재개
// 	UGameplayStatics::SetGamePaused(GetWorld(), false);
//
// 	// 입력 모드를 게임 전용으로 변경
// 	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
// 	if (PlayerController)
// 	{
// 		FInputModeGameOnly InputMode;
// 		PlayerController->SetInputMode(InputMode);
//
// 		// 마우스 커서 숨기기
// 		PlayerController->bShowMouseCursor = false;
// 	}
// }
