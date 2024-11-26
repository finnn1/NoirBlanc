// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PawnCardController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardClick, AActor*, HitActor);
class APawnCard;
class UPlayerUI;
class ANetworkPawn;
UCLASS()
class NOIRBLANC_API APawnCardController : public APlayerController
{
	GENERATED_BODY()

public:
	APawnCardController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:
	ANetworkPawn* PlayerPawn;
	
	//턴 제어 함수
	void SetTurnOwner(bool IsOwner);
	bool GetTurnOwner() const;
	
private:
	bool IsTurnOwner = false;
};
