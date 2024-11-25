// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "PawnCardController.generated.h"

/**
 * 
 */
class APawnCard;
class APawnCardGameMode;
class UInputMappingContext;
class UInputAction;
class UPlayerUI;
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
	TWeakObjectPtr<APawnCard> FirstSelectedCard;
	TWeakObjectPtr<APawnCard> SecondSelectedCard;

	UPROPERTY()
	APawnCard* TargetCard;
	
	//Input 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CardClickActioin;

	UPROPERTY()
	APawnCardGameMode* GameMode;
	
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UPlayerUI> TSubPlayerUI;
	UPROPERTY()
	UPlayerUI* PlayerUI; 

	UPROPERTY()
	int32 PlayerScore = 0;

public:
	//처음 시작
	UFUNCTION()
	void InitPlayerSettings();

	//턴 끝
	UFUNCTION()
	void PlayerTurnEnd();

	//카드 선택 함수
	void SelectCard(const FInputActionValue& Value);
	
	//카드의 매칭 여부 확인 함수
	UFUNCTION()
	bool IsCheckCardMatch(/*APawnCard* FirstSelectedCard, APawnCard* SecondSelectedCard*/);

	void InitPlayerUI();

	void SetTurnOwner(bool IsOwner);
	bool GetTurnOwner();

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_IncreaseScore();

	UFUNCTION()
	void IncreaseScore();
	
protected:

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

private:
	bool IsTurnOwner = false;
};
