// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DefaultCardDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class PawnType : uint8
{
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King
};

UENUM(BlueprintType)
enum class ColorType : uint8
{
	Blue,
	Red
};

UCLASS()
class NOIRBLANC_API UDefaultCardDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	
public:

	UPROPERTY(EditAnywhere)
	float U_Offset = 0.f;
	
	UPROPERTY(EditAnywhere)
	float V_Offset = 0.f;

	UFUNCTION()
	UMaterialInstance* GetCardMaterialIns();

	/*UFUNCTION()
	UMaterial* GetMatchCompleteMat();*/

private:
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* CardMaterialIns;

	/*UPROPERTY(EditDefaultsOnly)
	UMaterial* MatchCompleteMat;*/
};
