// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaggerUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UTaggerUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* JumpChargePower;

protected:
	virtual void NativeConstruct() override;

	
};
