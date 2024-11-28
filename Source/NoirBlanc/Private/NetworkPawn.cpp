// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnCardGameMode.h"
#include "PawnCard.h"
#include "PawnCardDataAsset.h"
#include "PawnCardController.h"
#include "PlayerUI.h"
#include "Net/UnrealNetwork.h"

class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ANetworkPawn::ANetworkPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartTurnFloat.BindUFunction(this, FName("StartTurnLerp"));
	EndTurnEvent.BindUFunction(this, FName("EndTurnLerp"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ANetworkPawn::BeginPlay()
{
	Super::BeginPlay();

	//턴 시작
	GameMode = Cast<APawnCardGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		//GameMode->OnTurnStart.AddDynamic(this, &ANetworkPawn::PlayerTurnStart);
		//GameMode->OnTurnEnd.AddDynamic(this, &ANetworkPawn::PlayerTurnEnd);
		GameMode->OnChangePlayerTurn.BindUObject(this, &ANetworkPawn::ChangePlayerTurn);
		GameMode->AddPlayer(this);
	}

	//TimelineComponent로 카드 확인
	if (MovingCurve)
	{
		Timeline->AddInterpFloat(MovingCurve, StartTurnFloat);
		Timeline->SetTimelineFinishedFunc(EndTurnEvent);
		
		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(0.3f);
	}

	//컨트롤러 캐싱
	if(APawnCardController* PcController = Cast<APawnCardController>(GetWorld()->GetFirstPlayerController()))
	{
		PawnCardContr = PcController;
	}

	if(IsLocallyControlled())
	{
		InitPlayerUI();

		// BeginPlay에서 서버가 먼저 시작되므로 Init으로 강제 Start
		if(HasAuthority())
		{
			SetIsTurnPlayer(true);
			PlayerUI->ShowTurnStart();
			//GameMode->InitPawnCardGame();
		}
		else
		{
			PlayerUI->ShowEnmTurnStart();
		}
	}
}

// Called every frame
void ANetworkPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString ThisName = this->GetName();
	
	//연결상태
	FString ConnStr = (GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection"));

	//Owner
	FString OwnerStr = GetOwner() ? GetOwner()->GetName() : TEXT("Not Owner");

	//권한
	FString role = UEnum::GetValueAsString<ENetRole>(GetLocalRole());
	
	//서버 권한인지 여부 체크
	FString bIsAuthority = HasAuthority() ? TEXT("Authority") : TEXT("No Authority");

	//내 것인지
	FString IsMine = IsLocallyControlled() ? TEXT("LocalControl") : TEXT("No Control");

	FString IsUI = (PlayerUI ? TEXT("Has UI") : TEXT("No UI"));

	FString IsTurn = (GetIsTurnPlayer() ? TEXT("Turn") : TEXT("No Turn"));

	FString LogStr = FString::Printf(TEXT("Connection : %s\nOwner : %s\nRole : %s\nAuthority : %s\nIsMine : %s\nPlayerUI : %s\nIsTurn : %s"),
		*ConnStr, *OwnerStr, *role, *bIsAuthority, *IsMine, *IsUI, *IsTurn);
	
	FVector TestLoc = FVector(500,500,300);
	if(IsLocallyControlled())
	{
		TestLoc += FVector(500,500,500);
	}

	//TextBaseActor를 this를 넣는지 여부는 상대좌표, 월드좌표?
	DrawDebugString(GetWorld(), TestLoc, LogStr, nullptr, FColor::Red, 0, true, 1);
}

// Called to bind functionality to input
void ANetworkPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if(UEnhancedInputComponent* EnhancedInputCompo = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputCompo->BindAction(CardClickAction, ETriggerEvent::Started, this, &ANetworkPawn::SelectCard);
	}
}



/*void ANetworkPawn::PlayerTurnStart()
{
	if(GetIsTurnPlayer())
	{
		FirstSelectedCard = nullptr;
		SecondSelectedCard = nullptr;	
	}

	UE_LOG(LogTemp, Warning, TEXT("-------------PlayerTurnStart--------------"));
}*/

/*void ANetworkPawn::PlayerTurnEnd()
{
	/*UE_LOG(LogTemp, Warning, TEXT("---------------------------"));
	CheckLog(this);#1#
	MulticastRPC_TurnEnd();
}*/


void ANetworkPawn::SelectCard(const FInputActionValue& Value)
{
	if(!GetIsTurnPlayer()) return;
	if(Timeline->IsPlaying()) return;

	FHitResult Hit;
	PawnCardContr->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if(Hit.bBlockingHit)
	{
		TargetCard = Cast<APawnCard>(Hit.GetActor());
		
		if(TargetCard)
		{
			if(FirstSelectedCard.IsValid())
			{
				//이미 첫 번째 카드를 선택했으면 체크
				SecondSelectedCard = TargetCard;
			}
			else
			{
				//아무 것도 선택 안 했으면 FirstSelectCard에 할당
				FirstSelectedCard = TargetCard;
			}
		
			//앞면으로 오픈
			Timeline->PlayFromStart();
			TargetCard->Selected();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Selected Card"));
		}
	}

}

bool ANetworkPawn::IsCheckCardMatch()
{
	bool bIsMatch = false;
	
	PawnCardType SelectedCardType = FirstSelectedCard.Get()->PawnCardData->PawnCardType;
	PawnCardType TargetCardType = SecondSelectedCard.Get()->PawnCardData->PawnCardType;
	
	if(SelectedCardType == TargetCardType)
	{
		bIsMatch = true;
		
		// 점수 증가
		IncreaseScore();
	}

	return bIsMatch;
}

void ANetworkPawn::InitPlayerUI()
{
	PlayerUI = Cast<UPlayerUI>(CreateWidget(GetWorld(), TSubPlayerUI));
	if(PlayerUI && !PlayerUI->IsInViewport())
	{
		PlayerUI->AddToViewport();
	}
}

void ANetworkPawn::ServerRPC_IncreaseEnemyScore_Implementation(ANetworkPawn* InstigatorPawn)
{
	MulticastRPC_IncreaseEnemyScore(InstigatorPawn);
}

void ANetworkPawn::MulticastRPC_IncreaseEnemyScore_Implementation(ANetworkPawn* InstigatorPawn)
{
	if(ANetworkPawn* ServerPawn = Cast<ANetworkPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		if(ServerPawn != InstigatorPawn)
		{
			if(ServerPawn->PlayerUI)
			{
				ServerPawn->PlayerUI->IncreaseEnemyScore();
			}	
		}
	}
}

void ANetworkPawn::IncreaseScore()
{
	//매칭에 성공했으면 자신의 UI에 점수를 올린다
	if(PlayerUI)
	{
		PlayerUI->IncreaseScore();
	}

	//자신을 제외한 다른 Pawn의 점수를 올린다
	ServerRPC_IncreaseEnemyScore(this);
}

void ANetworkPawn::ServerRPC_DestroyPawnCard_Implementation(APawnCard* FirstTargetCard, APawnCard* SecondTargetCard)
{
	if(!IsValid(FirstTargetCard) || !IsValid(SecondTargetCard)) return;

	FirstTargetCard->Destroy();
	SecondTargetCard->Destroy();
}


void ANetworkPawn::ServerRPC_GameEnd_Implementation()
{
	MulticastRPC_GameEnd();
}


void ANetworkPawn::MulticastRPC_GameEnd_Implementation()
{
	if(ANetworkPawn* ServerPawn = Cast<ANetworkPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		if(ServerPawn->PlayerUI)
		{
			ServerPawn->PlayerUI->ShowLosePlayer();
		}
	}
}

void ANetworkPawn::ServerRPC_ChangeTurn_Implementation(ANetworkPawn* EndPlayer)
{
	// 턴 교대
	GameMode->ChangeTurn(EndPlayer);
}

void ANetworkPawn::StartTurnLerp(float value)
{
	if(!TargetCard) return;
	
	FRotator FrontRotation = FRotator(0,180,0);
	FRotator BackRotation = FRotator::ZeroRotator;
	
	if(TargetCard->FrontBackState == CardState::Front)
	{
		//뒷면으로
		TargetCard->SetActorRotation(FRotator(0, FMath::Lerp(TargetCard->GetActorRotation().Yaw, BackRotation.Yaw, value),0));
		SecondSelectedCard = nullptr;
		
		if(FirstSelectedCard.IsValid() && FirstSelectedCard.Get()->FrontBackState == CardState::Front)
		{
			FirstSelectedCard.Get()->SetActorRotation(FRotator(0, FMath::Lerp(TargetCard->GetActorRotation().Yaw, BackRotation.Yaw, value),0));
		}
	}
	else
	{
		//SecondSelectedCard->TurnToBack();
		//EndTurn();
		
		//TODO 매칭 실패하면 상대에게 턴을 넘김 (Delegate)
		//앞면으로
		TargetCard->SetActorRotation(FRotator(0, FMath::Lerp(TargetCard->GetActorRotation().Yaw, FrontRotation.Yaw, value),0));
	}
}

void ANetworkPawn::EndTurnLerp()
{
	TargetCard->ChangeFrontBackState();
	if(SecondSelectedCard.IsValid())
	{
		if(IsCheckCardMatch())
		{
			// 매칭에 성공했으면 PawnCard 삭제
			ServerRPC_DestroyPawnCard(FirstSelectedCard.Get(), SecondSelectedCard.Get());
		}
		else
		{
			// 실패했으면 선택한 카드 원래대로
			Timeline->PlayFromStart();

			// 턴 교대 로직 시작
			ServerRPC_ChangeTurn(this);
		}
	}
	else
	{
		// 첫 번째 선택 카드 초기화
		if(FirstSelectedCard.IsValid())
		{
			if(FMath::IsNearlyEqual(FirstSelectedCard.Get()->GetActorRotation().Yaw, 0, 0.1))
			{
				FirstSelectedCard->ChangeFrontBackState();
				FirstSelectedCard = nullptr;
			}
		}
	}
}


void ANetworkPawn::CheckLog(ANetworkPawn* TargetPawn)
{
	FString ThisName = TargetPawn->GetName();
	
	//연결상태
	FString ConnStr = (TargetPawn->GetNetConnection() != nullptr ? TEXT("Valid Connection") : TEXT("Invalid Connection"));

	//Owner
	FString OwnerStr = TargetPawn->GetOwner() ? TargetPawn->GetOwner()->GetName() : TEXT("Not Owner");

	//권한
	FString role = UEnum::GetValueAsString<ENetRole>(TargetPawn->GetLocalRole());
	
	//서버 권한인지 여부 체크
	FString bIsAuthority = TargetPawn->HasAuthority() ? TEXT("Authority") : TEXT("No Authority");

	//내 것인지
	FString IsMine = TargetPawn->IsLocallyControlled() ? TEXT("LocalControl") : TEXT("No Control");

	FString IsUI = (TargetPawn->PlayerUI ? TEXT("Has UI") : TEXT("No UI"));

	FString IsTurn = (TargetPawn->GetIsTurnPlayer() ? TEXT("Turn") : TEXT("No Turn"));

	UE_LOG(LogTemp, Warning, TEXT("CheckLog -\nConnection : %s\nOwner : %s\nRole : %s\nAuthority : %s\nIsMine : %s\nPlayerUI : %s\nIsTurn : %s"),
		*ConnStr, *OwnerStr, *role, *bIsAuthority, *IsMine, *IsUI, *IsTurn)
}

void ANetworkPawn::ChangePlayerTurn(ANetworkPawn* StartPlayer)
{
	//델리게이트이므로 StartPlayer는 무조건 HasAuthority
	MulticastRPC_ChangePlayerTurn(StartPlayer);
}

void ANetworkPawn::MulticastRPC_ChangePlayerTurn_Implementation(ANetworkPawn* StartPlayer)
{
	// 다음 턴 플레이어가 로컬의 첫 번째 플레이어와 같으면 자신 턴
	ANetworkPawn* LocalNetPawn = Cast<ANetworkPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(LocalNetPawn)
	{
		if(StartPlayer == LocalNetPawn)
		{
			LocalNetPawn->PlayerUI->ShowTurnStart();
		}
		else
		{
			LocalNetPawn->PlayerUI->ShowEnmTurnStart();
		}
	}
}

void ANetworkPawn::SetIsTurnPlayer(bool IsTurn)
{
	//UE_LOG(LogTemp, Warning, TEXT("%d ,SetIsTurnPlayer : %d"), HasAuthority(), IsTurn);
	IsTurnPlayer = IsTurn;
}

bool ANetworkPawn::GetIsTurnPlayer()
{
	return IsTurnPlayer;
}

void ANetworkPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkPawn, IsTurnPlayer);
}

