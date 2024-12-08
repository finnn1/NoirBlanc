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
	// bind player controller to each pawn
	virtual void PostLogin(APlayerController* NewPlayer) override;

public: 
	UPROPERTY(VisibleAnywhere)
	TArray<class ACannon*> AllPlayers;

	int32 turnIdx = 0;
	void ChangeTurn();
	
};
