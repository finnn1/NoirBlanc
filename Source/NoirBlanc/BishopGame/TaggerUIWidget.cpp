// Fill out your copyright notice in the Description page of Project Settings.


#include "TaggerUIWidget.h"
#include "Components/ProgressBar.h"

void UTaggerUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	JumpChargePower->SetPercent(0.f);
}
