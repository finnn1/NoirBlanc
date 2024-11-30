// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameMode.h"
#include "FortressGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AFortressGameMode : public AGameMode
{
	GENERATED_BODY()

	AFortressGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
