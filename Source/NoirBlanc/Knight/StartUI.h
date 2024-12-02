// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "StartUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UStartUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_ServerReady;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_ClientReady;
	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Ready;

	virtual void NativeConstruct() override;

	void OnClickReady();
};
