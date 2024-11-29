// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KnightPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AKnightPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReady = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float TotalDistance = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
