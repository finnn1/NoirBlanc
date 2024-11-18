// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "PawnCardController.generated.h"

/**
 * 
 */
class APawnCard;
class UInputMappingContext;
class UInputAction;
UCLASS()
class NOIRBLANC_API APawnCardController : public APlayerController
{
	GENERATED_BODY()

public:
	APawnCardController();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

public:
	//TWeakObjectPtr<APawnCard> FirstSelectedCard;
	TWeakObjectPtr<APawnCard> FirstSelectedCard;
	TWeakObjectPtr<APawnCard> SecondSelectedCard;

	//Input 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CardClickActioin;

	//턴 시작
	void StartTurn();

	//턴 끝
	void EndTurn();

	//카드 선택 함수
	void SelectCard(const FInputActionValue& Value);

	//카드의 매칭 여부 확인 함수
	void IsCheckCardMatch(/*const UPawnCardDataAsset* TargetPawnCardData*/);
};
