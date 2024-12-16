// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingGameMode.h"

#include <string>

#include "KingCatcherPawn.h"
#include "KingCatcherWeapon.h"
#include "KingCharacter.h"
#include "KingUIUpdatable.h"
#include "NoirBlancGameInstance.h"
#include "SpawnLocation.h"
#include "TravelPlayerController.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "NoirBlanc/BishopGame/NoirBlancPlayerState.h"

AKingGameMode::AKingGameMode()
{
	TArray<AActor*> AllStartPointActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), AllStartPointActors);

	for (int i = 0; i < AllStartPointActors.Num(); ++i)
	{
		APlayerStart* _PlayerStart = Cast<APlayerStart>(AllStartPointActors[i]);
		if (_PlayerStart)
		{
			AllStartPoints.Add(_PlayerStart);
		}
	}
}

AActor* AKingGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	//// For Test ////
	ANoirBlancPlayerState* _PlayerState = Player->GetPlayerState<ANoirBlancPlayerState>();
	if (Player->IsLocalPlayerController())
	{
		_PlayerState->PieceColor = EPieceColor::White;
		_PlayerState->bIsAttaker = false;
		for (int i = 0; i < AllStartPoints.Num(); ++i)
		{
			if (AllStartPoints[i]->PlayerStartTag == TEXT("King"))
			{
				return AllStartPoints[i];
			}
		}
	}
	else
	{
		_PlayerState->PieceColor = EPieceColor::Black;
		_PlayerState->bIsAttaker = true;
		for (int i = 0; i < AllStartPoints.Num(); ++i)
		{
			if (AllStartPoints[i]->PlayerStartTag == TEXT("Catcher"))
			{
				return AllStartPoints[i];
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);


	//// TODO: 메인 게임에서는 주석 해제할 것. ////
	/*ANoirBlancPlayerState* _PlayerState = Player->GetPlayerState<ANoirBlancPlayerState>();
	UNoirBlancGameInstance* _NoirBlancGameInstance = GetGameInstance<UNoirBlancGameInstance>();

	// 게임모드 안에서 LocalPlayer다? 서버이다 => 하얀색
	if (Player->IsLocalPlayerController())
	{
		_PlayerState->PieceColor = EPieceColor::White;
		
		if (_NoirBlancGameInstance->AttackerColor == EPieceColor::White)
		{
			_PlayerState->bIsAttaker = true;
			for (int i = 0; i < AllStartPoints.Num(); ++i)
			{
				if (AllStartPoints[i]->PlayerStartTag == TEXT("Catcher"))
				{
					return AllStartPoints[i];
				}
			}
		}
		else
		{
			_PlayerState->bIsAttaker = false;
			for (int i = 0; i < AllStartPoints.Num(); ++i)
			{
				if (AllStartPoints[i]->PlayerStartTag == TEXT("King"))
				{
					return AllStartPoints[i];
				}
			}
		}
	}
	// 게임모드 안에서 LocalPlayer가 아니다? 클라이언트다 => 검은색
	else
	{
		_PlayerState->PieceColor = EPieceColor::Black;

		if (_NoirBlancGameInstance->AttackerColor == EPieceColor::Black)
		{
			_PlayerState->bIsAttaker = true;
			for (int i = 0; i < AllStartPoints.Num(); ++i)
			{
				if (AllStartPoints[i]->PlayerStartTag == TEXT("Catcher"))
				{
					return AllStartPoints[i];
				}
			}
		}
		else
		{
			_PlayerState->bIsAttaker = false;
			for (int i = 0; i < AllStartPoints.Num(); ++i)
			{
				if (AllStartPoints[i]->PlayerStartTag == TEXT("King"))
				{
					return AllStartPoints[i];
				}
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);*/
}

UClass* AKingGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ANoirBlancPlayerState* PlayerState = InController->GetPlayerState<ANoirBlancPlayerState>();
	if (PlayerState)
	{
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

	CurrentRemainTime = DefaultRemainTime;
}

void AKingGameMode::NotifyJoined(APlayerController* JoinedPlayer)
{
	JoinedPlayers.Add(JoinedPlayer);

	// 2명 들어왔으면 게임 시작 (GameMode에게 게임 시작하라고 알려주기)
	if (JoinedPlayers.Num() >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("두 명 다 들어옴!"));
		// 게임 시작
		// 1. 3, 2, 1 GO 타이머 고고
		GetWorld()->GetTimerManager().SetTimer
			(
			 StartCountDownTimerHandle,
			 this,
			 &AKingGameMode::StartCountTimer,
			 1.f,
			 true
			);
	}
}

void AKingGameMode::StartCountTimer()
{
	// 카운트 다운 내리자!
	if (CountdownNumber > -1)
	{
		--CountdownNumber;
		for (int i = 0; i < JoinedPlayers.Num(); ++i)
		{
			// UIUpdatable 인터페이스 구현 여부 확인
			if (JoinedPlayers[i]->GetPawn()->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
			{
				if (CountdownNumber > 0)
				{
					IKingUIUpdatable::Execute_MulticastRPC_UpdateStartCountdownUI
						(
						 JoinedPlayers[i]->GetPawn(),
						 FText::AsNumber(CountdownNumber)
						);
				}
				else
				{
					IKingUIUpdatable::Execute_MulticastRPC_UpdateStartCountdownUI
						(
						 JoinedPlayers[i]->GetPawn(),
						 FText::FromString(TEXT("GO!"))
						);
				}
			}
		}
	}

	// Countdown Number가 3, 2, 1, 0 땡하면
	// 게임을 시작하자!
	if (CountdownNumber == -1)
	{
		GetWorld()->GetTimerManager().ClearTimer(StartCountDownTimerHandle); // 타이머 클리어

		for (int i = 0; i < JoinedPlayers.Num(); ++i)
		{
			// UIUpdatable 인터페이스 구현 여부 확인
			if (JoinedPlayers[i]->GetPawn()->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
			{
				// 3. 모든 플레이어의 입력 제한 해제
				IKingUIUpdatable::Execute_MulticastRPC_SetInput(JoinedPlayers[i]->GetPawn(), true);

				// 4. Wait UI 삭제하고
				IKingUIUpdatable::Execute_MulticastRPC_UpdateStartCountdownUI(JoinedPlayers[i]->GetPawn(), FText());

				// 5. 게임 UI 띄우기
				IKingUIUpdatable::Execute_MulticastRPC_InitializeMainGameUI(JoinedPlayers[i]->GetPawn());

				// 6. Main Timer 똑딱똑딱 시작
				IKingUIUpdatable::Execute_MulticastRPC_UpdateMainTimerUI
					(
					 JoinedPlayers[i]->GetPawn(),
					 FText::AsNumber(CurrentRemainTime)
					);
				GetWorld()->GetTimerManager().SetTimer
					(
					 MainTimerHandle,
					 this,
					 &AKingGameMode::UpdateTimer,
					 1.f,
					 true
					);
			}
		}
	}
}

void AKingGameMode::UpdateTimer()
{
	CurrentRemainTime--;

	if (CurrentRemainTime < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);

		// Game Over
		for (int i = 0; i < JoinedPlayers.Num(); ++i)
		{
			if (JoinedPlayers[i]->GetPawn() == nullptr) return;

			if (JoinedPlayers[i]->GetPawn()->IsA(AKingCharacter::StaticClass()))
			{
				// Game Over 처리 (King Character 승리)
				GameOver(JoinedPlayers[i]->GetPawn());
			}
		}
		return;
	}

	for (int i = 0; i < JoinedPlayers.Num(); ++i)
	{
		// UIUpdatable 인터페이스 구현 여부 확인
		if (JoinedPlayers[i]->GetPawn()->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
		{
			IKingUIUpdatable::Execute_MulticastRPC_UpdateMainTimerUI
				(
				 JoinedPlayers[i]->GetPawn(),
				 FText::AsNumber(CurrentRemainTime)
				);
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

void AKingGameMode::OnKingCharacterOverlapped(AActor* OtherActor)
{
	const AKingCharacter* KingCharacter = Cast<AKingCharacter>(OtherActor);
	if (KingCharacter == nullptr) return;

	// Game Over : King Catcher Win
	for (const APlayerController* JoinedPlayer : JoinedPlayers)
	{
		if (JoinedPlayer->GetPawn()->IsA(AKingCatcherPawn::StaticClass()))
		{
			GameOver(JoinedPlayer->GetPawn());
			return;
		}
	}
}

void AKingGameMode::GameOver(APawn* Winner)
{
	GetWorld()->GetTimerManager().ClearTimer(StartCountDownTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("들어옴!"));
	// UIUpdatable 인터페이스 구현 여부 확인
	if (Winner->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
	{
		UNoirBlancGameInstance* _NoirBlancGameInstance = GetGameInstance<UNoirBlancGameInstance>();
		if (_NoirBlancGameInstance)
		{
			EPieceColor _WinnerColor = IKingUIUpdatable::Execute_GetPieceColor(Winner);
			for (int i = 0; i < JoinedPlayers.Num(); ++i)
			{
				// UIUpdatable 인터페이스 구현 여부 확인
				if (JoinedPlayers[i]->GetPawn()->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
				{
					IKingUIUpdatable::Execute_MulticastRPC_SetWinner(JoinedPlayers[i]->GetPawn(), _WinnerColor);
					//Level Travel
					// Cast<ATravelPlayerController>(GetWorld()->GetFirstPlayerController())->ServerRPC_LevelTravelToChess();
					// 승리자가 아닐 경우 Destory!
					// if (JoinedPlayers[i]->GetPawn() && JoinedPlayers[i]->GetPawn() != Winner)
					// {
					// 	JoinedPlayers[i]->GetPawn()->Destroy();
					// }
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NoirBlanc Game Instance Not Exist!!!"));
		}
	}
}
