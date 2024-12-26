// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "FortressGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AFortressGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AFortressGameMode();
	virtual void BeginPlay() override;
	
	// bind player controller to each pawn
	virtual void PostLogin(APlayerController* NewPlayer) override;

public: 
	UPROPERTY(VisibleAnywhere)
	TArray<class ACannon*> AllPlayers;

	void NewPlayerJoined(ACannon* NewPlayer);
	
	int32 turnIdx = 0;
	void ChangeTurn();
	
	void SetWind();
	
	UPROPERTY(EditAnywhere)
	TArray<int32> WindStrengthArray = {-3, -2, -1, 0, 1, 2, 3};

	UPROPERTY(EditAnywhere)
	float WindUnit = 30.0f;
		
	int32 WindArrayIndex = 0;
	
public:
	FTimerHandle CountdownTimer;

	int32 CountdownLeft = 3;
	
	void StartCountdown();
};
