// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
public:
	AActor* TraceForActor();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_LevelTravel(const FString& LevelName);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Quit();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Quit();
	void Quit();
};