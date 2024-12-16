// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KingGameMainUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UKingGameMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Timer;
};
