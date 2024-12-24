// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FortressGameState.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AFortressGameState : public AGameStateBase
{
	GENERATED_BODY()

	void NextRound(class ACannon* OwnerCannon, class UFortressUI* FortressUI);
};