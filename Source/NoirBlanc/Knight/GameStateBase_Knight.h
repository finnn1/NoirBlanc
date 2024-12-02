// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CountDownUI.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateBase_Knight.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AGameStateBase_Knight : public AGameStateBase
{
	GENERATED_BODY()
	AGameStateBase_Knight();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void StartCountDown();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateCountDownUI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCountDownUI> CountDownFactory;
	UCountDownUI* CountDownUI;

	UPROPERTY(ReplicatedUsing=OnRep_CountDownLeft)
	int32 CountDownLeft = 3;

	UFUNCTION()
	void OnRep_CountDownLeft();

	void CountDown();

	FTimerHandle Handle;

	UPROPERTY(Replicated)
	bool Started = false;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool Finished = false;

	void StartTimer();
	UPROPERTY(Replicated)
	int32 GameTimeLeft = 60;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateFinishUI();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFinishUI> FinishUIFactory;
	UFinishUI* FinishUI;
	UPROPERTY(Replicated)
	FText Winner;
};
