// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CountDownUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UCountDownUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	

public:
	UPROPERTY()
	UTextBlock* Txt_Count;
	
};
