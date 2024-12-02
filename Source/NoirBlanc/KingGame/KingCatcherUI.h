// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KingCatcherUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UKingCatcherUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;
};
