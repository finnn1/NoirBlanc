// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputMappingContext.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Knight.generated.h"

class UCountDownUI;

UCLASS()
class NOIRBLANC_API APlayer_Knight : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Knight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle Handle;

	// --------------------------------------------------------------------------------
	//
	// Input
	//

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Player;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Jump;
	
	// --------------------------------------------------------------------------------
	//
	// Network
	//
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	int32 ConnectedPlayers;
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartGame();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateCountDown();
	
	// --------------------------------------------------------------------------------
	//
	// Movement
	//
	void Move(const FInputActionValue& Value);

	void Jump();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurDistance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PreviousDistance = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float TotalDistance = 0;
	
	// --------------------------------------------------------------------------------
	//
	// UI
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFinishUI> FinishUIFactory;
	UFinishUI* FinishUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> StartUI;
	class UUserWidget* Start;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMainUI> MainUI;
	class UMainUI* Main;

	// --------------------------------------------------------------------------------
	//
	// Calculate Distance
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayer_Knight* OtherPlayer;
	void FindOtherPlayer();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateDistanceUI(float serverDistance, float clientDistance);

	// --------------------------------------------------------------------------------
	//
	// CountDown
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCountDownUI> CountDownFactory;
	UCountDownUI* CountDownUI;
	UFUNCTION()
	void OnRep_CountDownLeft();
	UPROPERTY(ReplicatedUsing=OnRep_CountDownLeft)
	int32 CountDownLeft = 3;
	void CountDown();

	// --------------------------------------------------------------------------------
	//
	// Timer
	//
	UPROPERTY(Replicated, EditAnywhere)
	int32 TimeLeft = 30;
	void StartTimer();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateTimerUI();
	
	
	// --------------------------------------------------------------------------------
	//
	// Logic
	//	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Started = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Ended = false;

	bool Finished = false;
	
	// --------------------------------------------------------------------------------
	//
	// Player Data
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Piece;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Team;
};
