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
DECLARE_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_MULTICAST_DELEGATE(FOnShuffleStart);
DECLARE_MULTICAST_DELEGATE(FOnShuffleEnd);
DECLARE_DELEGATE_OneParam(FOnChangePlayerTurn, ANetworkPawn*);
DECLARE_DELEGATE_OneParam(FOnGameSet, ANetworkPawn*)
UCLASS()
class NOIRBLANC_API APawnCardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APawnCardGameMode();

	virtual void BeginPlay() override;
	FTimerHandle TimerHandle;
	UPROPERTY(EditDefaultsOnly)
	ANetworkPawn* TurnNetPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawnCardSpawner> TSubCardSpawner;

	UPROPERTY(VisibleAnywhere)
	TArray<APawnCard*> PawnCards;

	UPROPERTY(VisibleAnywhere)
	TArray<ANetworkPawn*> Players;

	UPROPERTY(VisibleAnywhere)
	APawnCardSpawner* CardSpawner;
	
	UPROPERTY()
	int32 TurnPlayerIdx = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InitGameSettings")
	int32 PlayerNum = 2;

	UPROPERTY()
	int32 CurrentPlayerNum = 0;

	FOnGameStart OnGameStart;
	FOnShuffleStart OnShuffleStart;
	FOnShuffleEnd OnShuffleEnd;
	
	FOnChangePlayerTurn OnChangePlayerTurn;
	FOnGameSet OnGameSet;

	void SetInitCardSetting();
	
	void InitPawnCardGame();
	
	void AddPlayer(ANetworkPawn* Player);

	bool CheckRemainCards();
	void GameSet();

	void ChangeTurn(ANetworkPawn* EndPlayer);

	void StartPost();

	void DeleteCardFromMap(APawnCard* SelectedCard);
	
};
