// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class APlayer_Knight;
/**
 * 
 */
UCLASS()
class NOIRBLANC_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	void NativeConstruct() override;
	void NativePreConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	class UTextBlock* Text_Timer;
	void UpdateTimerText(float time);
	
	class UTextBlock* Text_MyDistance;
	void UpdateMyDistance(float distance);
	
	class UTextBlock* Text_EnemyDistance;
	void UpdateEnemyDistance(float distance);
};
