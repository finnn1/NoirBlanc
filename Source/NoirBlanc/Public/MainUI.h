// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void UpdateTimerText(float time);

	class UTextBlock* Text_Timer;
	float Timer = 10.f;
};
