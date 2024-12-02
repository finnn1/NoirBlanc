// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PieceTypes.h"
#include "NoirBlancGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UNoirBlancGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor DeffenderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceType DeffenderType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor AttackerColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceType AttackerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor WinnerColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<EPieceType> BoardTypeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<EPieceColor> BoardColorData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<int32> EncounterCountData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<int32 > MoveCountData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	EPieceColor Turn = EPieceColor::Black;
};
