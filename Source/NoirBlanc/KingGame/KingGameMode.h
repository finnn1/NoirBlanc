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

	TArray<APlayerController*> JoinedPlayers;
	
	// 클릭 가능한 모든 위치
	TArray<class ASpawnLocation*> AllSpawnLocations;

	// 현재 클릭한 Location의 Index
	TArray<int32> SelectedLocationsIndex;

	TArray<APlayerStart*> AllStartPoints;
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
	FTimerHandle StartCountDownTimerHandle;
	void NotifyJoined(APlayerController* JoinedPlayer);
	void StartCountTimer();
	int32 CountdownNumber = 4;

	FTimerHandle MainTimerHandle;
	void UpdateTimer();
	int32 CurrentRemainTime;
	
	UPROPERTY(EditAnywhere)
	int32 DefaultRemainTime = 100;

	void HandleCatcherLocationSelect(int32 ButtonIndex, class ASpawnLocation* ClickedSpawnLocation);
	void ColorSpawnLocationToPlayer(int32 SpawnLocationIndex, APlayerController* PlayerController);
	void HandleCatcherFireButtonClick();
	void FireAt(TArray<class ASpawnLocation*> SpawnLocations);

	void OnKingCharacterOverlapped(AActor* OtherActor);
	
	void GameOver(APawn* Winner);
};
