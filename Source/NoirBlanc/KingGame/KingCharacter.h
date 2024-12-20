// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KingGameMainUI.h"
#include "KingUIUpdatable.h"
#include "GameFramework/Character.h"
#include "NoirBlanc/BishopGame/WaitingOtherPlayerUI.h"
#include "NoirBlanc/Knight/CountDownUI.h"
#include "NoirBlanc/Knight/WaitingUI.h"
#include "KingCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class NOIRBLANC_API AKingCharacter : public ACharacter, public IKingUIUpdatable
{
	GENERATED_BODY()

public:
	AKingCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* KingMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// About Join
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<UWaitingOtherPlayerUI> UWaitingOtherPlayerUIClass;
	// UWaitingOtherPlayerUI* WaitingOtherPlayerUI;
	// 서버에게 들어왔다고 알려주기
	void Joined(APlayerController* NewPlayer);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Joined(APlayerController* JoinedPlayer);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ShowWaitingUI(APlayerController* JoinedPlayer);

	/// UIUpdatble Interface ///
	virtual EPieceColor GetPieceColor_Implementation() override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_UpdateStartCountdownUI(const FText& NewText) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetInput(bool bIsEnable) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKingGameMainUI> KingGameMainUIClass;
	
	UKingGameMainUI* KingGameMainUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWaitingUI> WaitingUIClass;
	
	UWaitingUI* WaitingUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCountDownUI> CountDownUIClass;
	
	UCountDownUI* CountDownUI;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_InitializeMainGameUI() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_UpdateMainTimerUI(const FText& NewText) override;

	// GameOver UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFinishUI> FinishUIClass;
	
	class UFinishUI* FinishUI;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_ShowGameOverUI(const FText& Winner) override;
	
	// Set GameInstance
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetWinner(EPieceColor WinnerColor) override;
	
};
