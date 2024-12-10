// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FireBoostWidget.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UFireBoostWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Impulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxVelocity = 4000;
	
	UFUNCTION(BlueprintCallable)
	float ChargeVelocity();
};
