// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBase_Knight.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AGameModeBase_Knight : public AGameModeBase
{
	GENERATED_BODY()
	virtual void BeginPlay() override;

	// TODO : 다른 레벨에서 다른 레벨로 이동할 때 잘 동작하는지 확인하기
	//virtual void PostLogin(APlayerController* NewPlayer) override;
	int connectedClients;
};
