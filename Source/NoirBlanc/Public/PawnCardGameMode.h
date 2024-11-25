// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PawnCardGameMode.generated.h"

class APawnCard;
class APawnCardController;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeScore);
UCLASS()
class NOIRBLANC_API APawnCardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	APawnCardController* TurnPlayer;

	UPROPERTY(VisibleAnywhere)
	TArray<APawnCard*> PawnCards;

	UPROPERTY(VisibleAnywhere)
	TArray<APawnCardController*> Players;

	int32 TurnPlayerIdx = 0;

	FOnTurnStart OnTurnStart;
	FOnTurnEnd OnTurnEnd;
	FOnChangeScore OnChangeScore;
	
	void InitPawnCardGame();
	
	void AddPlayerController(APawnCardController* Player);
	
	void TurnStart();
	void TurnEnd(APawnCardController* EndPlayer);

	void ChangeScore();
	void ChangePlayerTurn();

	void GameEnd();
};
