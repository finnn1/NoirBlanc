// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KingGameMode.generated.h"

UCLASS()
class NOIRBLANC_API AKingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKingGameMode();

	// 클릭 가능한 모든 위치
	TArray<class ASpawnLocation*> AllSpawnLocations;

	TArray<AActor*> AllStartPoints;
	int StartPointOrder = -1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKingCharacter> KingCharacterClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKingCatcherPawn> CatcherPawnClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKingCatcherWeapon> CatcherWeaponClass;

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual void BeginPlay() override;

public:
	void FireAt(TArray<class ASpawnLocation*> SpawnLocations);
};
