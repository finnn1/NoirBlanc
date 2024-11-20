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
UCLASS()
class NOIRBLANC_API APawnCardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	FOnTurnStart OnTurnStart;
	FOnTurnEnd OnTurnEnd;
	
	UPROPERTY(EditDefaultsOnly)
	APawnCardController* TurnPlayer;

	UPROPERTY(VisibleAnywhere)
	TArray<APawnCard*> PawnCards;
	
	void InitPawnCardGame();
	
	void TurnStart();
	void TurnEnd();
	void ChangePlayerTurn();

	void GameEnd();
};
