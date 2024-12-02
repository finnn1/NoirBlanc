// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardController.h"
#include "ControllerUI.h"
#include "Blueprint/UserWidget.h"

APawnCardController::APawnCardController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APawnCardController::BeginPlay()
{
	Super::BeginPlay();

	if(CntrUIFactory)
	{
		CntrUI = Cast<UControllerUI>(CreateWidget(GetWorld(), CntrUIFactory));
		CntrUI->AddToViewport();
	}
	
	//마우스 커서 보이게 하기
	bShowMouseCursor = true;
}

void APawnCardController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

void APawnCardController::GameStart()
{
	if(CntrUI) CntrUI->ShowStartText();
}

void APawnCardController::StartShuffleText()
{
	if(CntrUI) CntrUI->HideStartText();
}

void APawnCardController::HideShuffleText()
{
	if(CntrUI) CntrUI->HideShuffleText();
}
