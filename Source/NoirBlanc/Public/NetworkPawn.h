// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "NoirBlanc/Knight/TurnUI.h"
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

	//카드 선택 함수
	UFUNCTION()
	void SelectCard(const FInputActionValue& Value);
	
	//카드의 매칭 여부 확인 함수
	UFUNCTION()
	bool IsCheckCardMatch();

	void InitPlayerUI();

	void IncreaseScore(bool IsNoLuck);
	
	//void SetScore(bool IsMine);
	void SetIsTurnPlayer(bool IsTurn);
	bool GetIsTurnPlayer();

	void CheckLog(ANetworkPawn* TargetPawn);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SelectCard(APawnCard* SelectedCard);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SelectCard(APawnCard* SelectedCard);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetWinnerInstance(EPieceColor WinnerColor);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 네트워크 통신
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_GameStart();
	
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_ShuffleStart();

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_ShuffleEnd();

	// 매칭 된 카드 파괴
	UFUNCTION(Server, reliable)
	void ServerRPC_DestroyPawnCard(APawnCard* FirstTargetCard, APawnCard* SecondTargetCard);

	// 점수 반영
	UFUNCTION(Server, reliable)
	void ServerRPC_IncreaseScore(ANetworkPawn* ScorePlayer, bool IsNoLuck);

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_IncreaseScore(ANetworkPawn* ScorePlayer, int32 Score);

	// 턴 교대
	UFUNCTION(Server, reliable)
	void ServerRPC_ChangeTurn(ANetworkPawn* EndPlayer);

	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_ChangePlayerTurn(ANetworkPawn* StartPlayer);

	// 게임 끝
	UFUNCTION(NetMulticast, reliable)
	void MulticastRPC_GameEnd(ANetworkPawn* WinnerPlayer);
	
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

	UFUNCTION()
	void ChangePlayerTurn(ANetworkPawn* StartPlayer);

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPT_Test(APawnCard* FirstTargetCard, APawnCard* SecondTargetCard);


private:
	UPROPERTY(Replicated)
	bool IsTurnPlayer = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTurnUI> TurnUIFactory;

	UPROPERTY()
	UTurnUI* TurnUI;

	UPROPERTY(VisibleAnywhere, Replicated)
	EPieceColor PawnPieceColor;

	UFUNCTION()
	void DtyCard(APawnCard* DestroyCard);
};
