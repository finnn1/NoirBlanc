// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PieceTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NoirBlancGameInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_TwoParams(FAddSession, int32, FString);
DECLARE_DELEGATE_OneParam(FFindComplete, bool);
UCLASS()
class NOIRBLANC_API UNoirBlancGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor WinnerColor = EPieceColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor DeffenderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceType DeffenderType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor AttackerColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceType AttackerType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<EPieceType> BoardTypeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<EPieceColor> BoardColorData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<int32> EncounterCountData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<int32 > MoveCountData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	EPieceColor Saved_Turn = EPieceColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 AttackerRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 AttackerCol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 DeffenderRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	int32 DeffenderCol;

	FName SelectedGame;
public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_LevelTravel(const FString& LevelName);

	// 세션 생성
	UFUNCTION()
	void CreateSession(FString DisplayName, FName SelectedGame);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션 파괴
	UFUNCTION()
	void DestroySession(FString SessionName);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션 검색
	UFUNCTION()
	void FindOtherSession();
	void OnFindOtherSessionComplete(bool bWasSuccessful);

	// 세션 참여
	void JoinOtherSession(int32 Index);
	void OnJoinOtherSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// 세션의 모든 처리를 진행
	IOnlineSessionPtr SessionInterFace;

	// 세션 검색 처리
	TSharedPtr<FOnlineSessionSearch> SessionSearchResult;

	// 세션이 검색되었을 때 각 세션의 정보를 전달해주는 딜리게이트
	FAddSession  OnAddSession;

	// 세션 검색이 완전 완료 되었을 때 전달해주는 딜리게이트
	FFindComplete OnFindComplete;
};
