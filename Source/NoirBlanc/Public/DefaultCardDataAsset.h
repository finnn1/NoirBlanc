// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DefaultCardDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class PawnCardType : uint8
{
	NoLuck,
	PawnRed,
	RookRed,
	KnightRed,
	BishopRed,
	QueenRed,
	KingRed,
	PawnBlue,
	RookBlue,
	KnightBlue,
	BishopBlue,
	QueenBlue,
	KingBlue
};
UCLASS()
class NOIRBLANC_API UDefaultCardDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	PawnCardType PawnCardType;

	UPROPERTY(EditAnywhere)
	float U_Offset = 0.f;
	
	UPROPERTY(EditAnywhere)
	float V_Offset = 0.f;

	UFUNCTION()
	UMaterialInstance* GetCardMaterialIns();

	UFUNCTION()
	UMaterial* GetMatchCompleteMat();

private:
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* CardMaterialIns;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* MatchCompleteMat;
};
