// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "NetworkPawn.generated.h"

class APawnCardController;
class UInputMappingContext;
class UInputAction;
class APawnCard;
class APawnCardGameMode;
class UPlayerUI;
UCLASS()
class NOIRBLANC_API ANetworkPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANetworkPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	TWeakObjectPtr<APawnCard> FirstSelectedCard;
	TWeakObjectPtr<APawnCard> SecondSelectedCard;

	UPROPERTY()
	APawnCardController* PawnCardContr;

	UPROPERTY()
	APawnCard* TargetCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CardClickAction;

	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UPlayerUI> TSubPlayerUI;
	UPROPERTY()
	UPlayerUI* PlayerUI;

	UPROPERTY()
	APawnCardGameMode* GameMode;

	//처음 시작
	/*UFUNCTION()
	void PlayerTurnStart();

	//턴 끝
	UFUNCTION()
	void PlayerTurnEnd();*/

	//카드 선택 함수
	UFUNCTION()
	void SelectCard(const FInputActionValue& Value);
	
	//카드의 매칭 여부 확인 함수
	UFUNCTION()
	bool IsCheckCardMatch();

	void InitPlayerUI();

	void IncreaseScore();
	
	//void SetScore(bool IsMine);
	void SetIsTurnPlayer(bool IsTurn);
	bool GetIsTurnPlayer();

	void CheckLog(ANetworkPawn* TargetPawn);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 네트워크 통신
	UFUNCTION(Server, reliable)
	void ServerRPC_DestroyPawnCard(APawnCard* FirstTargetCard, APawnCard* SecondTargetCard);
	
	UFUNCTION(Server, reliable)
	void ServerRPC_IncreaseEnemyScore(ANetworkPawn* InstigatorPawn);
	
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_IncreaseEnemyScore(ANetworkPawn* InstigatorPawn);

	UFUNCTION(Server, reliable)
	void ServerRPC_ChangeTurn(ANetworkPawn* EndPlayer);

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_ChangePlayerTurn(ANetworkPawn* StartPlayer);

	UFUNCTION(Server, reliable)
	void ServerRPC_GameEnd();

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_GameEnd();
	
	
	// 타임라인 이벤트
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

	//TODO Controller에서 받은 턴 제어 변수

	UFUNCTION()
	void ChangePlayerTurn(ANetworkPawn* StartPlayer);
	

private:
	UPROPERTY(Replicated)
	bool IsTurnPlayer = false;
};
