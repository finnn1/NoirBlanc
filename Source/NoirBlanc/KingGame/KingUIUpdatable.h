// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KingUIUpdatable.generated.h"

enum class EPieceColor : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKingUIUpdatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOIRBLANC_API IKingUIUpdatable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	EPieceColor GetPieceColor();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_UpdateStartCountdownUI(const FText& NewText) = 0;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetInput(bool bIsEnable) = 0;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_InitializeMainGameUI() = 0;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_UpdateMainTimerUI(const FText& NewText) = 0;

	// Set GameInstance
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetWinner(EPieceColor WinnerColor) = 0;
};
