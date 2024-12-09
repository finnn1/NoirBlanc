// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingOtherPlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UWaitingOtherPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MainText;
	
public:
	void SetText(const FText& NewText);
};
