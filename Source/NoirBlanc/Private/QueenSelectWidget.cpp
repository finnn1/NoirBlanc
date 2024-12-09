// Fill out your copyright notice in the Description page of Project Settings.


#include "QueenSelectWidget.h"
#include "ChessBoard.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UQueenSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Pawn->OnClicked.AddDynamic(this, &UQueenSelectWidget::OnPawnClicked);
	Btn_Knight->OnClicked.AddDynamic(this, &UQueenSelectWidget::OnKnightClicked);
	Btn_Rook->OnClicked.AddDynamic(this, &UQueenSelectWidget::OnRookClicked);
	Btn_Bishop->OnClicked.AddDynamic(this, &UQueenSelectWidget::OnBishopClicked);
	
	AActor* Board = UGameplayStatics::GetActorOfClass(GetWorld(), AChessBoard::StaticClass());
	AChessBoard* ChessBoard = Cast<AChessBoard>(Board);

	OnBtnClicked.BindUObject(ChessBoard, &AChessBoard::QueenWidgetClicked);
}

void UQueenSelectWidget::OnPawnClicked()
{
	SelectedLevel = TEXT("Pawn");
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if(OnBtnClicked.IsBound())
	{
		OnBtnClicked.Execute(SelectedLevel);
	}
}

void UQueenSelectWidget::OnKnightClicked()
{
	SelectedLevel = TEXT("Knight");
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if(OnBtnClicked.IsBound())
	{
		OnBtnClicked.Execute(SelectedLevel);
	}
}

void UQueenSelectWidget::OnRookClicked()
{
	SelectedLevel = TEXT("Rook");
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if(OnBtnClicked.IsBound())
	{
		OnBtnClicked.Execute(SelectedLevel);
	}
}

void UQueenSelectWidget::OnBishopClicked()
{
	SelectedLevel = TEXT("Bishop");
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if(OnBtnClicked.IsBound())
	{
		OnBtnClicked.Execute(SelectedLevel);
	}
}