// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UGameEndUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void UpdateWinnerText(FText winner);
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Txt_Winner;
};