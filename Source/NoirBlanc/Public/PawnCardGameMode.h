// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PawnCardGameMode.generated.h"

class APawnCard;
class ANetworkPawn;
class APawnCardSpawner;
class APawnCardController;
/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnChangePlayerTurn, ANetworkPawn*);
DECLARE_DELEGATE_OneParam(FOnGameSet, ANetworkPawn*)
UCLASS()
class NOIRBLANC_API APawnCardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	ANetworkPawn* TurnNetPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawnCardSpawner> TSubCardSpawner;

	UPROPERTY(VisibleAnywhere)
	TArray<APawnCard*> PawnCards;

	UPROPERTY(VisibleAnywhere)
	TArray<ANetworkPawn*> Players;

	int32 TurnPlayerIdx = 0;
	
	FOnChangePlayerTurn OnChangePlayerTurn;
	FOnGameSet OnGameSet;
	
	void InitPawnCardGame();
	
	void AddPlayer(ANetworkPawn* Player);

	bool CheckRemainCards();
	void GameSet();

	void ChangeTurn(ANetworkPawn* EndPlayer);
};
