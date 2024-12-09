// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PieceTypes.h"
#include "GameFramework/PlayerState.h"
#include "NoirBlancPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API ANoirBlancPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	EPieceColor PieceColor = EPieceColor::Blank;
	
	UPROPERTY(Replicated)
	bool bIsAttaker = true;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
