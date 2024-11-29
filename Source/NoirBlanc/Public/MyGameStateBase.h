// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	AMyGameStateBase();	
	
public:
	UPROPERTY(Replicated)
	float remainingTime = 10;
	
	UFUNCTION(BlueprintCallable)
	void StartTimer();
	FTimerHandle handle;
	void UpdateRemainingTime();

	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CreateFinishUI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFinishUI> FinishUIFactory;
	UFinishUI* FinishUI;
	
	bool TimeOver = false;

	UPROPERTY(Replicated)
	FText Winner;
};
