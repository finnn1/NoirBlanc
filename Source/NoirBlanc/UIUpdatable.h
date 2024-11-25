// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIUpdatable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIUpdatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOIRBLANC_API IUIUpdatable
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_SetRandomText() = 0;

	virtual void UpdateText(const FText& InputedText) = 0;
	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_UpdateText(const FText& InputedText) = 0;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SpawnWeapon(FVector Location, FRotator Rotation, UClass* WeaponClass) = 0;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetUITextTo(const FText& InputedText, const FText& NewText, const TArray<bool>& StringCorrectArray) = 0;
};
