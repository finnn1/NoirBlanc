// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WaitingUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UWaitingUI : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Waiting;
	void DestroyWaitingUI();

private:
	void RemoveUI();
};
