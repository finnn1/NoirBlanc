// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BishopGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API ABishopGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
