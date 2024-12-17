// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AngleWidget.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UAngleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class URadialSlider* RadialSlider;
};
