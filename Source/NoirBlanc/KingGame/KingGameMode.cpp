// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingGameMode.h"
#include "KingCatcherPawn.h"
#include "KingCatcherWeapon.h"
#include "KingCharacter.h"
#include "SpawnLocation.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "NoirBlanc/BishopGame/NoirBlancPlayerState.h"

AKingGameMode::AKingGameMode()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), AllStartPoints);
}

AActor* AKingGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	ANoirBlancPlayerState* _PlayerState = Player->GetPlayerState<ANoirBlancPlayerState>();

	// TODO: Chess판 완성되면 수정할 부분 //
	if (StartPointOrder >= AllStartPoints.Num()) return Super::ChoosePlayerStart_Implementation(Player);
	if (StartPointOrder < 1) StartPointOrder++;

	if (_PlayerState)
	{
		// Bishop
		if (StartPointOrder == 0)
		{
			_PlayerState->bIsAttaker = false;
		}
		else
		{
			_PlayerState->bIsAttaker = true;
		}
	}
	////////////////////////////////////////////////

	UE_LOG(LogTemp, Warning, TEXT("Inside ChoosePlayerStart Function"));

	return AllStartPoints[StartPointOrder];
}

UClass* AKingGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("GetDefaultPawnClassForController : %s"), *InController->GetActorNameOrLabel());

	ANoirBlancPlayerState* PlayerState = InController->GetPlayerState<ANoirBlancPlayerState>();
	if (PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState : %d"), PlayerState->bIsAttaker);
		// If King
		if (PlayerState->bIsAttaker == false)
		{
			return KingCharacterClass;
		}
		// If Catcher
		else
		{
			return CatcherPawnClass;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState : NULL"));
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AKingGameMode::BeginPlay()
{
	Super::BeginPlay();

	// When the game starts, find all SpawnLocations and put them into an array~!
	TArray<AActor*> AllFoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnLocation::StaticClass(), AllFoundActors);
	for (AActor* AllFoundActor : AllFoundActors)
	{
		ASpawnLocation* SpawnLocation = Cast<ASpawnLocation>(AllFoundActor);
		if (SpawnLocation)
		{
			AllSpawnLocations.Add(SpawnLocation);
		}
	}
}

void AKingGameMode::FireAt(TArray<ASpawnLocation*> SpawnLocations)
{
	for (ASpawnLocation* SpawnLocation : SpawnLocations)
	{
		FActorSpawnParameters SpawnParameters;
		FVector FireLocation = SpawnLocation->FrontArrow->GetComponentLocation();
		FRotator FireRotation = SpawnLocation->FrontArrow->GetComponentRotation();

		// TODO: FireLocation에다가 무기 소환해버리기
		GetWorld()->SpawnActor<AKingCatcherWeapon>(CatcherWeaponClass, FireLocation, FireRotation, SpawnParameters);
	}
}
