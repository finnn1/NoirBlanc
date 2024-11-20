// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "PawnCardController.generated.h"

/**
 * 
 */
class APawnCard;
class APawnCardGameMode;
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

	UPROPERTY()
	APawnCard* TargetCard;
	
	UPROPERTY()
	APawnCard* TempCard;

	//Input 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CardClickActioin;

	UPROPERTY()
	APawnCardGameMode* GameMode;

	UPROPERTY()
	int32 PlayerScore = 0;

public:
	//처음 시작
	UFUNCTION()
	void InitPlayerSettings();

	//턴 끝
	void TurnEnd();

	//카드 선택 함수
	void SelectCard(const FInputActionValue& Value);
	
	//카드의 매칭 여부 확인 함수
	bool IsCheckCardMatch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* MovingCurve;
	
	FOnTimelineFloat StartTurnFloat;
	FOnTimelineEvent EndTurnEvent;

	UFUNCTION()
	void StartTurnLerp(float value);

	UFUNCTION()
	void EndTurnLerp();

	void SetTurnOwner(bool IsOwner);
	bool GetTurnOwner();

	void ReturnCardBack(APawnCard* PawnCard);

private:
	bool IsTurnOwner = false;
};
