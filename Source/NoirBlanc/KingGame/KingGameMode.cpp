// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingGameMode.h"

#include <string>

#include "KingCatcherPawn.h"
#include "KingCatcherUI.h"
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
#include "NoirBlanc/Knight/FinishUI.h"

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
	// ANoirBlancPlayerState* _PlayerState = Player->GetPlayerState<ANoirBlancPlayerState>();
	// if (Player->IsLocalPlayerController())
	// {
	// 	_PlayerState->PieceColor = EPieceColor::White;
	// 	_PlayerState->bIsAttaker = false;
	// 	for (int i = 0; i < AllStartPoints.Num(); ++i)
	// 	{
	// 		if (AllStartPoints[i]->PlayerStartTag == TEXT("King"))
	// 		{
	// 			return AllStartPoints[i];
	// 		}
	// 	}
	// }
	// else
	// {
	// 	_PlayerState->PieceColor = EPieceColor::Black;
	// 	_PlayerState->bIsAttaker = true;
	// 	for (int i = 0; i < AllStartPoints.Num(); ++i)
	// 	{
	// 		if (AllStartPoints[i]->PlayerStartTag == TEXT("Catcher"))
	// 		{
	// 			return AllStartPoints[i];
	// 		}
	// 	}
	// }
	//
	// return Super::ChoosePlayerStart_Implementation(Player);
	
	//// TODO: 메인 게임에서는 주석 해제할 것. ////
	ANoirBlancPlayerState* _PlayerState = Player->GetPlayerState<ANoirBlancPlayerState>();
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

	return Super::ChoosePlayerStart_Implementation(Player);
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
						 FText::FromString(TEXT("시작!"))
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

void AKingGameMode::HandleCatcherLocationSelect(int32 ButtonIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("index : %d"), ButtonIndex));

	// 이미 배열에 있으면 리턴 (이미 선택한 곳이면)
	for (int32 SelectedLocationIndex : SelectedLocationsIndex)
	{
		if (SelectedLocationIndex == ButtonIndex) return;
	}

	// 4개 이미 선택했으면 리턴
	if (SelectedLocationsIndex.Num() >= 4) return;


	// 선택한 인덱스 목록에 추가
	SelectedLocationsIndex.Add(ButtonIndex);

	// 해당 인덱스 선택했다고 CatcherPawn에게 빨간 불 들어오게 하기.
	for (int i = 0; i < JoinedPlayers.Num(); ++i)
	{
		if (JoinedPlayers[i]->GetPawn()->IsA(AKingCatcherPawn::StaticClass()))
		{
			AKingCatcherPawn* _KingCatcherPawn = Cast<AKingCatcherPawn>(JoinedPlayers[i]->GetPawn());
			if (_KingCatcherPawn == nullptr) continue;

			_KingCatcherPawn->MulticastRPC_SetButtonColor(ButtonIndex, true);
		}
	}
}

void AKingGameMode::HandleCatcherFireButtonClick()
{
	// 클릭한 인덱스 처리
	// TODO: 서버에게 클릭한 SpawnLocation 보내서 처리하도록 하기
	// 1. 클라이언트 Pawn 쪽에서 클릭 했다는 사실을 서버 Pawn에게 알림
	// 2. 서버 Pawn에서 클릭 트레이싱을 해서 FHitResult를 구한다.
	// 3. 서버 Pawn에서 어떤 SpawnLocation이 맞았는지 배열로 쌓아놓는다.
	// 4. 4개 이상 쌓이면 더 이상 못 쌓게 하기.

	// 화면의 Go! 버튼을 누르면 해당 위치에서 레이저 발사!
	// 서버 Pawn 쪽에서 갯수 4개가 차면 서버 KingGameMode로 보내서 처리하도록 하기!

	// if (SelectedSpawnLocations.Num() < 4) return;

	TArray<ASpawnLocation*> SelectedSpawnLocations;

	for (class ASpawnLocation* SpawnLocation : AllSpawnLocations)
	{
		for (int32 SelectedLocationIndex : SelectedLocationsIndex)
		{
			if (SpawnLocation->Index == SelectedLocationIndex)
			{
				SelectedSpawnLocations.Add(SpawnLocation);
				SpawnLocation->ColorToRed();
			}
		}
	}

	// Input 막기
	AKingCatcherPawn* CatcherPawnCache = nullptr;
	for (int i = 0; i < JoinedPlayers.Num(); ++i)
	{
		if (JoinedPlayers[i]->GetPawn()->IsA(AKingCatcherPawn::StaticClass()))
		{
			AKingCatcherPawn* _KingCatcherPawn = Cast<AKingCatcherPawn>(JoinedPlayers[i]);
			if (_KingCatcherPawn == nullptr) continue;
			CatcherPawnCache = _KingCatcherPawn;
			_KingCatcherPawn->MulticastRPC_SetInput(false);
		}
	}

	// 1초 동안 King Character에게 보여준 다음 무기 발사
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
		(
		 TimerHandle,
		 FTimerDelegate::CreateLambda
		 (
		  [this, SelectedSpawnLocations, CatcherPawnCache]()
		  {
			  for (class ASpawnLocation* SelectedSpawnLocation : SelectedSpawnLocations)
			  {
				  SelectedSpawnLocation->ColorToWhite();
			  }

			  AKingGameMode* _KingGameMode = Cast<AKingGameMode>(this->GetWorld()->GetAuthGameMode());
			  if (_KingGameMode)
			  {
				  // TODO: 게임모드에서 로직 처리
				  _KingGameMode->FireAt(SelectedSpawnLocations);
			  }

			  // SelectedSpawnLocations.Empty();

			  if (CatcherPawnCache)
			  {
				  CatcherPawnCache->MulticastRPC_SetInput(true);
			  }
		  }
		 ),
		 1.f,
		 false
		);
}

void AKingGameMode::FireAt(TArray<ASpawnLocation*> SpawnLocations)
{
	for (ASpawnLocation* SpawnLocation : SpawnLocations)
	{
		FActorSpawnParameters SpawnParameters;
		FVector FireLocation = SpawnLocation->FrontArrow->GetComponentLocation();
		FRotator FireRotation = SpawnLocation->FrontArrow->GetComponentRotation();

		// 무기 소환
		GetWorld()->SpawnActor<AKingCatcherWeapon>(CatcherWeaponClass, FireLocation, FireRotation, SpawnParameters);

		// Catcher UI 버튼 선택한거 원래 색으로
		for (int i = 0; i < JoinedPlayers.Num(); ++i)
		{
			if (JoinedPlayers[i]->GetPawn()->IsA(AKingCatcherPawn::StaticClass()))
			{
				AKingCatcherPawn* _KingCatcherPawn = Cast<AKingCatcherPawn>(JoinedPlayers[i]->GetPawn());
				if (_KingCatcherPawn == nullptr) continue;

				for (int32 SelectedLocationIndex : SelectedLocationsIndex)
				{
					_KingCatcherPawn->MulticastRPC_SetButtonColor(SelectedLocationIndex, false);
				}

				// 선택한 Index 배열 초기화
				SelectedLocationsIndex.Empty();
			}
		}
	}
}

void AKingGameMode::OnKingCharacterOverlapped(AActor* OtherActor)
{
	const AKingCharacter* KingCharacter = Cast<AKingCharacter>(OtherActor);
	if (IsValid(KingCharacter) == false) return;

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

	// 게임오버 UI 띄우기
	// UIUpdatable 인터페이스 구현 여부 확인
	for (int i = 0; i < JoinedPlayers.Num(); ++i)
	{
		APawn* _Pawn = JoinedPlayers[i]->GetPawn();
		if (IsValid(_Pawn))
		{
			if (_Pawn->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
			{
				EPieceColor _WinnerColor = IKingUIUpdatable::Execute_GetPieceColor(Winner);

				if (_WinnerColor == EPieceColor::White)
				{
					IKingUIUpdatable::Execute_MulticastRPC_ShowGameOverUI(_Pawn, FText::FromString("Blanc"));
				}
				else if (_WinnerColor == EPieceColor::Black)
				{
					IKingUIUpdatable::Execute_MulticastRPC_ShowGameOverUI(_Pawn, FText::FromString("Noir"));				
				}
				else
				{
					IKingUIUpdatable::Execute_MulticastRPC_ShowGameOverUI(_Pawn, FText::FromString("Draw"));
				}
			}
		}
	}

	// 5초 뒤에 체스 보드로 돌아가기.
	FTimerHandle GameOverUITimerHandle;
	GetWorld()->GetTimerManager().SetTimer
		(
		 GameOverUITimerHandle,
		 FTimerDelegate::CreateLambda([this, Winner]()
		 {
			 // GAMEOVER and Return to Chessboard.
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
						 APawn* _Pawn = JoinedPlayers[i]->GetPawn();
						 if (IsValid(_Pawn))
						 {
							 if (_Pawn->GetClass()->ImplementsInterface(UKingUIUpdatable::StaticClass()))
							 {
								 IKingUIUpdatable::Execute_MulticastRPC_SetWinner(_Pawn, _WinnerColor);

								 // Level Travel
								 ATravelPlayerController* _ATravelPlayerController = Cast<
									 ATravelPlayerController>(GetWorld()->GetFirstPlayerController());
								 if (_ATravelPlayerController)
								 {
									 _ATravelPlayerController->ServerRPC_LevelTravelToChess();
								 }
								 else
								 {
									 UE_LOG(LogTemp, Error, TEXT("Travel Controller 설정하세요!"));
								 }

								 // 승리자가 아닐 경우 Destory!
								 // if (JoinedPlayers[i]->GetPawn() && JoinedPlayers[i]->GetPawn() != Winner)
								 // {
								 // 	JoinedPlayers[i]->GetPawn()->Destroy();
								 // }
							 }
						 }
					 }
				 }
				 else
				 {
					 UE_LOG(LogTemp, Warning, TEXT("NoirBlanc Game Instance Not Exist!!!"));
				 }
			 }
		 }),
		 5.f,
		 false
		);
}
