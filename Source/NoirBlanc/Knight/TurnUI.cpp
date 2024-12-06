// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnUI.h"

void UTurnUI::NativeConstruct()
{
	Super::NativeConstruct();
	Turn = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Turn")));
}

void UTurnUI::ShowTurn()
{
	Turn->SetRenderOpacity(1);
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UTurnUI::HideTurn, 1, false);
}

void UTurnUI::HideTurn()
{
	Turn->SetRenderOpacity(0);
	GetWorld()->GetTimerManager().ClearTimer(Handle);
}
