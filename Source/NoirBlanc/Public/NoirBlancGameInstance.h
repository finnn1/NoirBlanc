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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor DeffenderColor;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceType DeffenderType;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor AttackerColor;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceType AttackerType;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor WinnerColor;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<EPieceType> BoardTypeData;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<EPieceColor> BoardColorData;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<int32> EncounterCountData;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<int32 > MoveCountData;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	EPieceColor Turn = EPieceColor::Black;
};
