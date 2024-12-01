// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnCardDataAsset.generated.h"

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
/**
 * 
 */
UCLASS()
class NOIRBLANC_API UPawnCardDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	PawnCardType PawnCardType;

	UPROPERTY(EditAnywhere)
	int32 TotalNum;
	
	UPROPERTY(EditAnywhere)
	UMaterialInstance* CardMaterialIns;

	UPROPERTY(EditAnywhere)
	float U_Offset = 0.f;
	
	UPROPERTY(EditAnywhere)
	float V_Offset = 0.f;
};
