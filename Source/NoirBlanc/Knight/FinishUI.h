// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FinishUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UFinishUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_Winner;
public:
	void UpdateWinnerText(FText winner);
};
