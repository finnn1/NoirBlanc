// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnUI.h"

#include "Components/CanvasPanelSlot.h"

void UTurnUI::NativeConstruct()
{
	Super::NativeConstruct();
	Turn_Blanc = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Turn_Blanc")));
	Turn_Noir = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Turn_Noir")));
}

void UTurnUI::ShowTurn(EPieceColor color)
{
	if(color == EPieceColor::White)
	{
		UCanvasPanelSlot* blancSlot = Cast<UCanvasPanelSlot>(Turn_Blanc->Slot);
		blancSlot->SetPosition(FVector2d(-212, 108));
		//GetWorld()->GetTimerManager().SetTimer(Handle, this, &UTurnUI::HideBlanc, 1, false);

		UCanvasPanelSlot* noirSlot = Cast<UCanvasPanelSlot>(Turn_Noir->Slot);
		noirSlot->SetPosition(FVector2d(-560, 248));
	}
	else
	{
		UCanvasPanelSlot* blancSlot = Cast<UCanvasPanelSlot>(Turn_Blanc->Slot);
		blancSlot->SetPosition(FVector2d(-560, 108));
		//GetWorld()->GetTimerManager().SetTimer(Handle, this, &UTurnUI::HideBlanc, 1, false);

		UCanvasPanelSlot* noirSlot = Cast<UCanvasPanelSlot>(Turn_Noir->Slot);
		noirSlot->SetPosition(FVector2d(-212, 248));
	}
}

void UTurnUI::HideBlanc()
{
	//Turn_Blanc->SetRenderOpacity(0);
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Turn_Blanc->Slot);
	slot->SetPosition(FVector2d(-212, 108));
	GetWorld()->GetTimerManager().ClearTimer(Handle);
}
