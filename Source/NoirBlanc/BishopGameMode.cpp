// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopGameMode.h"
#include "BishopGamePlayerController.h"
#include "BishopPawn.h"
#include "NoirBlancPlayerState.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "Weapon.h"

ABishopGameMode::ABishopGameMode()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), AllStartPoints);
}

AActor* ABishopGameMode::ChoosePlayerStart_Implementation(AController* Player)
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

UClass* ABishopGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("GetDefaultPawnClassForController : %s"), *InController->GetActorNameOrLabel());

	ANoirBlancPlayerState* PlayerState = InController->GetPlayerState<ANoirBlancPlayerState>();
	if (PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState : %d"), PlayerState->bIsAttaker);
		// If Bishop
		if (PlayerState->bIsAttaker == false)
		{
			return BishopPawnClass;
		}
		// If Tagger
		else
		{
			return TaggerPlayerClass;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState : NULL"));
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ABishopGameMode::BeginPlay()
{
	Super::BeginPlay();

	FText RandomText = PickRandomText();
	CurrentTextToType = RandomText;
}

bool ABishopGameMode::CheckCommittedText(const FText& TypedText)
{
	// 현재 입력한 문장이 제시된 문장과 일치하는지 검사
	return CurrentTextToType.EqualTo(TypedText);
}

void ABishopGameMode::UpdateInputtedText(const FText& TypedText)
{
	TArray<AActor*> AllPlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, ABishopGamePlayerController::StaticClass(), AllPlayerControllers);
	for (const AActor* PlayerController : AllPlayerControllers)
	{
		const ABishopGamePlayerController* BishopGamePlayerController = Cast<ABishopGamePlayerController>(
			PlayerController);
		if (BishopGamePlayerController)
		{
			ABishopPawn* BishopPawn = Cast<ABishopPawn>(BishopGamePlayerController->GetPawn());
			if (BishopPawn)
			{
				BishopPawn->MulticastRPC_SetUITextTo(TypedText, CurrentTextToType);
			}

			ATP_ThirdPersonCharacter* TaggerCharacter = Cast<ATP_ThirdPersonCharacter>(
				BishopGamePlayerController->GetPawn());
			if (TaggerCharacter)
			{
				TaggerCharacter->MulticastRPC_SetUITextTo(TypedText, CurrentTextToType);
			}
		}
	}
}

void ABishopGameMode::CommitText(const FText& TypedText)
{
	bool bIsCorrect = CheckCommittedText(TypedText);
	UE_LOG(LogTemp, Warning, TEXT("%s / %p"), *TypedText.ToString(),
	       *CurrentTextToType.ToString());
	// 친 문장이 맞는 경우
	if (bIsCorrect)
	{
		FText _RandomText = PickRandomText(); // 새로운 문장 생성
		
		TArray<AActor*> AllPlayerControllers;
		UGameplayStatics::GetAllActorsOfClass(this, ABishopGamePlayerController::StaticClass(), AllPlayerControllers);
		
		for (const AActor* PlayerController : AllPlayerControllers)
		{
			const ABishopGamePlayerController* BishopGamePlayerController = Cast<ABishopGamePlayerController>(
				PlayerController);
			if (BishopGamePlayerController)
			{
				FVector SpawnPoint = FVector();
				FRotator SpawnRotation = FRotator();
				ABishopPawn* BishopPawn = Cast<ABishopPawn>(BishopGamePlayerController->GetPawn());
				if (BishopPawn)
				{
					// 무기 소환!
					SpawnPoint = BishopPawn->WeaponSpawnPoint->GetComponentLocation();
					SpawnRotation = BishopPawn->WeaponSpawnPoint->GetComponentRotation();
					BishopPawn->MulticastRPC_SpawnWeapon(SpawnPoint, SpawnRotation, BishopWeaponClass);

					// 비숍 플레이어에게 클리어 및 다음 문장 제시하기
					BishopPawn->MulticastRPC_SetUITextTo(FText::FromString(""), _RandomText);
				}

				ATP_ThirdPersonCharacter* TaggerCharacter = Cast<ATP_ThirdPersonCharacter>(
					BishopGamePlayerController->GetPawn());
				if (TaggerCharacter)
				{
					// 무기 소환!
					// -> 근데 엔진에서 Replicated 체크 해줘가지고 걍 한쪽에서만 소환해줘도 자동 동기화 됨. 나이스
					// if (!SpawnPoint.IsNearlyZero())
					// {
					// 	TaggerCharacter->MulticastRPC_SpawnWeapon(SpawnPoint, SpawnRotation, BishopWeaponClass);
					// }

					// Tagger Player에게 다음 문장 업데이트
					TaggerCharacter->MulticastRPC_SetUITextTo(FText::FromString(""), _RandomText);
				}
			}
		}
	}
	// 친 문장이 틀린 경우
	else
	{
		// TODO: 무엇을 할까?
	}
}


FText ABishopGameMode::PickRandomText()
{
	if (TextsToType.IsEmpty()) return FText();

	FText RandomText;
	do
	{
		const int32 MaxNum = TextsToType.Num();
		const int32 RandomNumber = FMath::RandRange(0, MaxNum - 1);
		RandomText = TextsToType[RandomNumber];
		UE_LOG(LogTemp, Warning, TEXT("Current Text: %s"), *CurrentTextToType.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Random Text: %s"), *RandomText.ToString());
	}
	while (TextsToType.Num() >= 2 && RandomText.EqualTo(CurrentTextToType));
	CurrentTextToType = RandomText;

	return RandomText;
}
