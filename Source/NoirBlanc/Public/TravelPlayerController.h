// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TravelPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API ATravelPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRPC_LevelTravelToChess();

	UFUNCTION(Client, Reliable)
	void ClientRPC_SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());
};
