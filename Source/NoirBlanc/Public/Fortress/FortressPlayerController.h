// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FortressPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AFortressPlayerController : public APlayerController
{
	GENERATED_BODY()

	void BeginPlay() override;
	
};
