// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoTextUI.generated.h"

/**
 * 
 */

UCLASS()
class NOIRBLANC_API UInfoTextUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Color;

	virtual void NativeConstruct() override;
};