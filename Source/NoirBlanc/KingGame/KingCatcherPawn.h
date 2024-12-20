// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KingUIUpdatable.h"
#include "GameFramework/Pawn.h"
#include "NoirBlanc/BishopGame/WaitingOtherPlayerUI.h"
#include "NoirBlanc/KingGame/KingGameMainUI.h"
#include "NoirBlanc/Knight/CountDownUI.h"
#include "NoirBlanc/Knight/WaitingUI.h"
#include "KingCatcherPawn.generated.h"

UCLASS()
class NOIRBLANC_API AKingCatcherPawn : public APawn, public IKingUIUpdatable
{
	GENERATED_BODY()

public:
	AKingCatcherPawn();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CatcherMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ClickAction;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	// UI Management
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UKingCatcherUI> CatcherUIClass;
	
	class UKingCatcherUI* CatcherUI;

	UFUNCTION()
	void HandleFireButtonClick();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_HandleFireButtonClick();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetButtonColor(int32 ButtonIndex, bool IsSelected);

	// Spawn Management
	// TArray<class ASpawnLocation*> SelectedSpawnLocations;
	
	// Player Join
	void Joined(APlayerController* NewPlayer);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_Joined(APlayerController* JoinedPlayer);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ShowWaitingUI(APlayerController* JoinedPlayer);
	
	// Input Handling
	// void Click(const struct FInputActionValue& Value);
	
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_Click(FVector WorldLocation, FVector WorldDirection);
	
	void HandleButtonClick(int32 ButtonIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_HandleButtonClick(int32 ButtonIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Select(ASpawnLocation* SpawnLocation);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Deselect(ASpawnLocation* SpawnLocation);
	
	// UIUpdatable Interface Implementation
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
	
	// Game State
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetWinner(EPieceColor WinnerColor) override;
};
