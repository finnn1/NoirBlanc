// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnUI.h"

#include "Components/CanvasPanelSlot.h"

void UTurnUI::NativeConstruct()
{
	Super::NativeConstruct();
	Turn_Blanc = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Turn_Blanc")));
	Turn_Noir = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Turn_Noir")));
	
	Text_Blanc = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Blanc")));
	Text_Noir = Cast<UTextBlock>(GetWidgetFromName(TEXT("Txt_Noir")));
}

void UTurnUI::ShowTurn(EPieceColor color)
{
	if(color == EPieceColor::White)
	{
		PlayAnimation(Anim_Blanc);
	}
	else
	{
		PlayAnimation(Anim_Noir);
	}
}

void UTurnUI::HideBlanc()
{
	//Turn_Blanc->SetRenderOpacity(0);
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Turn_Blanc->Slot);
	slot->SetPosition(FVector2d(-212, 108));
	GetWorld()->GetTimerManager().ClearTimer(Handle);
}
