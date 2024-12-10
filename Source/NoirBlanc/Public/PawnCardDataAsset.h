// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultCardDataAsset.h"
#include "PawnCardDataAsset.generated.h"

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
class NOIRBLANC_API UPawnCardDataAsset : public UDefaultCardDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	PawnCardType PawnCardType;

	UPROPERTY(EditAnywhere)
	int32 TotalNum;
	
	UPROPERTY(EditAnywhere)
	int32 Score;

	UFUNCTION()
	void InitMeshMaterial(UStaticMeshComponent* StaticMeshComp);

	UFUNCTION()
	void SetMatchingMaterial(UStaticMeshComponent* StaticMeshComp, float Alpha);
};
