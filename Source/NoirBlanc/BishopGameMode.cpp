// Fill out your copyright notice in the Description page of Project Settings.

#include "BishopGameMode.h"
#include "BishopPawn.h"
#include "NoirBlancPlayerState.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "BishopWeapon.h"

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
	TArray<bool> StringCorrectArray = CheckTypingCorrect(TypedText);

	TArray<AActor*> AllPawns;
	UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), AllPawns);
	for (AActor* _Pawn : AllPawns)
	{
		// UIUpdatable 인터페이스 구현 여부 확인
		if (_Pawn && _Pawn->GetClass()->ImplementsInterface(UUIUpdatable::StaticClass()))
		{
			// 복붙 했는지 검사
			bool bIsCheatting = CheckCheatting(TypedText);
			if (bIsCheatting)
			{
				// 복붙했으면 입력한 내용 다 초기화 시켜버리기
				UE_LOG(LogTemp, Warning, TEXT("Don't cheat!! GO AWAY!!!!"));
				IUIUpdatable::Execute_MulticastRPC_SetUITextTo(_Pawn, FText::FromString(""), CurrentTextToType, TArray<bool>());
				CurrentTypedText = TEXT("");
			}
			else
			{
				CurrentTypedText = TypedText.ToString();
				// 메시지 호출
				IUIUpdatable::Execute_MulticastRPC_SetUITextTo(_Pawn, TypedText, CurrentTextToType, StringCorrectArray);
			}
		}
	}
}

void ABishopGameMode::PickRandomTextAndUpdateUI()
{
	FText RandomText = PickRandomText();

	TArray<AActor*> AllPawns;
	UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), AllPawns);
	for (AActor* _Pawn : AllPawns)
	{
		// UIUpdatable 인터페이스 구현 여부 확인
		if (_Pawn && _Pawn->GetClass()->ImplementsInterface(UUIUpdatable::StaticClass()))
		{
			// 메시지 호출
			IUIUpdatable::Execute_MulticastRPC_SetUITextTo(_Pawn, FText::FromString(""), CurrentTextToType, TArray<bool>());
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

		TArray<AActor*> AllPawns;
		UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), AllPawns);
		for (AActor* _Pawn : AllPawns)
		{
			ABishopPawn* BishopPawn = Cast<ABishopPawn>(_Pawn);
			if (BishopPawn)
			{
				// 무기 소환!
				FVector SpawnPoint = BishopPawn->WeaponSpawnPoint->GetComponentLocation();
				FRotator SpawnRotation = BishopPawn->WeaponSpawnPoint->GetComponentRotation();
				BishopPawn->MulticastRPC_SpawnWeapon(SpawnPoint, SpawnRotation, BishopWeaponClass);
			}

			// UIUpdatable 인터페이스 구현 여부 확인
			if (_Pawn && _Pawn->GetClass()->ImplementsInterface(UUIUpdatable::StaticClass()))
			{
				// 메시지 호출
				IUIUpdatable::Execute_MulticastRPC_SetUITextTo(_Pawn, FText::FromString(""), _RandomText, TArray<bool>());
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

void ABishopGameMode::GameOver(APawn* Winner, APawn* Loser)
{
}

bool ABishopGameMode::CheckCheatting(const FText& TypedText)
{
	// 이미 입력된 것을 한번에 지우는 것은 제외
	if (CurrentTypedText.Len() > TypedText.ToString().Len())
	{
		return false;
	}

	// 다른 곳에서 바로 복붙하는 것을 방지
	// Commit된 텍스트가 방금 전 Update 된 Text보다 2 ~ 3글자 이상 차이 난다면?
	// -> 복붙했을 가능성이 높음
	int Difference = FMath::Abs(TypedText.ToString().Len() - CurrentTypedText.Len());
	if (Difference >= 2)
	{
		return true; // Cheatting!!
	}
	return false; // not cheatting
}

void ABishopGameMode::OnButtonPressed()
{
	// Destory Bishop Pawn!
	TArray<AActor*> AllBishopPawns;
	UGameplayStatics::GetAllActorsOfClass(this, ABishopPawn::StaticClass(), AllBishopPawns);
	for (AActor* _BishopPawn : AllBishopPawns)
	{
		_BishopPawn->Destroy();
	}

	// TODO: Game Over 처리 (Tagger 승리)
}

TArray<bool> ABishopGameMode::CheckTypingCorrect(const FText& TypedText)
{
	TArray<bool> StringCorrectArray;
	for (int i = 0; i < TypedText.ToString().Len() && i < CurrentTextToType.ToString().Len(); i++)
	{
		if (TypedText.ToString()[i] == CurrentTextToType.ToString()[i])
		{
			StringCorrectArray.Add(true);
		}
		else
		{
			StringCorrectArray.Add(false);
		}
	}

	return StringCorrectArray;
}