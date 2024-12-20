// Fill out your copyright notice in the Description page of Project Settings.


#include "InfoUI.h"
#include "Components/Button.h"
#include "InfoTextUI.h"

void UInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_Info->OnHovered.AddDynamic(this, &UInfoUI::ShowOnHovered);
	Btn_Info->OnUnhovered.AddDynamic(this, &UInfoUI::HideOnUnhovered);
}

void UInfoUI::ShowOnHovered()
{
	InfoTextUI = CreateWidget<UInfoTextUI>(GetWorld(), InfoTextUIClass);
	InfoTextUI->AddToViewport();
}

void UInfoUI::HideOnUnhovered()
{
	InfoTextUI->RemoveFromParent();
}