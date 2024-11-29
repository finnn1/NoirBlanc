// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TempLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API ATempLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTempLobbyUI> TempLobbyClass;
};
