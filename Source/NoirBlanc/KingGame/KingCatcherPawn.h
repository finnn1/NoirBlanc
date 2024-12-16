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

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UKingCatcherUI> CatcherUIClass;
	
	class UKingCatcherUI* CatcherUI;

	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnConfirmButtonClicked();
	
	TArray<class ASpawnLocation*> SelectedSpawnLocations;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CatcherMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ClickAction;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
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

	
	// About Input
	void Click(const struct FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Click(FVector WorldLocation, FVector WorldDirection);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SelectForAll(ASpawnLocation* SpawnLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeselectForAll(ASpawnLocation* SpawnLocation);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SelectOnlyForLocallyPlayer(ASpawnLocation* SpawnLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeselectOnlyForLocallyPlayer(ASpawnLocation* SpawnLocation);


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

	// Set GameInstance
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetWinner(EPieceColor WinnerColor) override;
};
