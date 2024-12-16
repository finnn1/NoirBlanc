// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPawn.h"
#include "Components/DecalComponent.h"
#include "ControllerUI.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnCardGameMode.h"
#include "PawnCard.h"
#include "PawnCardDataAsset.h"
#include "PawnCardController.h"
#include "PlayerUI.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "NoirBlancGameInstance.h"
#include "Engine/DecalActor.h"

class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ANetworkPawn::ANetworkPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartTurnFloat.BindUFunction(this, FName("StartTurnLerp"));
	EndTurnEvent.BindUFunction(this, FName("EndTurnLerp"));

	/*WhiteDecalCompo = CreateDefaultSubobject<UDecalComponent>(TEXT("WhiteDecalCompo"));
	BlackDecalCompo = CreateDefaultSubobject<UDecalComponent>(TEXT("BlackDecalCompo"));

	WhiteDecalCompo->SetupAttachment(RootComponent);
	BlackDecalCompo->SetupAttachment(RootComponent);

	if(MatWhiteDecal)
	{
		WhiteDecalCompo->SetDecalMaterial(MatWhiteDecal);	
	}

	if(MatBlackDecal)
	{
		BlackDecalCompo->SetDecalMaterial(MatBlackDecal);	
	}*/
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ANetworkPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//턴 시작
	GameMode = Cast<APawnCardGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->OnGameStart.AddUObject(this, &ANetworkPawn::MulticastRPC_GameStart);
		GameMode->OnShuffleStart.AddUObject(this, &ANetworkPawn::MulticastRPC_ShuffleStart);
		GameMode->OnShuffleEnd.AddUObject(this, &ANetworkPawn::MulticastRPC_ShuffleEnd);
		GameMode->OnGameSet.BindUObject(this, &ANetworkPawn::MulticastRPC_GameEnd);
		GameMode->OnChangePlayerTurn.BindUObject(this, &ANetworkPawn::ChangePlayerTurn);
		//GameMode->AddPlayer(this);
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
	APawnCardController* PcController = Cast<APawnCardController>(GetWorld()->GetFirstPlayerController());
	if(PcController)
	{
		PawnCardContr = PcController;
	}

	if(IsLocallyControlled())
	{
		InitPlayerUI();
	}
}

// Called every frame
void ANetworkPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FString ThisName = this->GetName();
	
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

	int32 Score = (GetPlayerState() ? GetPlayerState()->GetScore() : -1);

	const UEnum* EnumPtr = StaticEnum<EPieceColor>();
	FString EnumName = EnumPtr->GetNameByValue(static_cast<int64>(PawnPieceColor)).ToString();

	FString IsPlaying = (Timeline->IsPlaying() ? TEXT("Playing") : TEXT("Not Playing"));
	
	FString LogStr = FString::Printf(TEXT("Connection : %s\nOwner : %s\nRole : %s\nAuthority : %s\nIsMine : %s\nPlayerUI : %s\nIsTurn : %s\nIsPlaying : %s"),
		*ConnStr, *OwnerStr, *role, *bIsAuthority, *IsMine, *IsUI, *IsTurn, *IsPlaying);
	
	FVector TestLoc = GetActorLocation();
	TestLoc.Y += -200;
	if(IsLocallyControlled())
	{
		TestLoc.X += -100;
	}

	//TextBaseActor를 this를 넣는지 여부는 상대좌표, 월드좌표?
	DrawDebugString(GetWorld(), TestLoc, LogStr, nullptr, FColor::Red, 0, true, 1);*/
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
		if(CardClickAction)
		{
			EnhancedInputCompo->BindAction(CardClickAction, ETriggerEvent::Started, this, &ANetworkPawn::SelectCard);	
		}
	}
}


void ANetworkPawn::MulticastRPC_SetWinnerInstance_Implementation(EPieceColor WinnerColor)
{
	UNoirBlancGameInstance* NB_GM = Cast<UNoirBlancGameInstance>(GetGameInstance());
	if(!NB_GM) return;

	NB_GM->WinnerColor = WinnerColor;
	FString ColorStr = StaticEnum<EPieceColor>()->GetDisplayNameTextByValue(static_cast<int64>(WinnerColor)).ToString();

	if(HasAuthority())
	{
		Cast<APawnCardController>(GetWorld()->GetFirstPlayerController())->ServerRPC_LevelTravelToChessBoard();
		/*FTimerHandle WinnerHandle;
		GetWorldTimerManager().SetTimer(WinnerHandle, [this]()
		{
			Cast<APawnCardController>(GetWorld()->GetFirstPlayerController())->ServerRPC_LevelTravelToChessBoard();
		}, 1.5f, false);*/
	}
}

void ANetworkPawn::DrawDecalActor(FVector DecalLocation, EPieceColor ContrColor)
{
	FVector ReLoatedLocation = DecalLocation;
	ReLoatedLocation.Y += -20;
	ReLoatedLocation.Z += 30;
	
	ADecalActor* DecalActor = GetWorld()->SpawnActor<ADecalActor>(ReLoatedLocation, FRotator::ZeroRotator);
	if(ContrColor == EPieceColor::White && MatWhiteDecal)
	{
		DecalActor->SetDecalMaterial(MatWhiteDecal);
	}
	else if(ContrColor == EPieceColor::Black && MatBlackDecal)
	{
		DecalActor->SetDecalMaterial(MatBlackDecal);
	}
	
	DecalActor->SetActorRotation(FRotator(-3.f,-98.f, 100.f));
	DecalActor->SetActorScale3D(FVector(0.15f,0.2f,0.15f));
}

void ANetworkPawn::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetworkPawn, IsTurnPlayer);
	DOREPLIFETIME(ANetworkPawn, PawnPieceColor);
}

void ANetworkPawn::MulticastRPC_GameStart_Implementation()
{
	if(GetOwner())
	{
		APawnCardController* Cntr = Cast<APawnCardController>(GetOwner());
		if(Cntr && Cntr->CntrUI)
		{
			Cntr->CntrUI->ShowStartText();
		}
	}
}

void ANetworkPawn::MulticastRPC_ShuffleEnd_Implementation()
{
	if(GetOwner())
	{
		APawnCardController* Cntr = Cast<APawnCardController>(GetOwner());
		if(Cntr && Cntr->CntrUI)
		{
			Cntr->CntrUI->HideShuffleText();
		}
	}
}

void ANetworkPawn::MulticastRPC_ShuffleStart_Implementation()
{
	if(GetOwner())
	{
		APawnCardController* Cntr = Cast<APawnCardController>(GetOwner());
		if(Cntr && Cntr->CntrUI)
		{
			Cntr->CntrUI->HideStartText();
			Cntr->CntrUI->ShowShuffleText();
		}
	}
}


void ANetworkPawn::SelectCard(const FInputActionValue& Value)
{
	if(!GetIsTurnPlayer()) return;
	
	FHitResult Hit;
	if(PawnCardContr)
	{
		PawnCardContr->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	}

	if(Hit.bBlockingHit)
	{
		APawnCard* SelectedCard = Cast<APawnCard>(Hit.GetActor());
		
		if(SelectedCard)
		{
			// 카드 회전 중 다른 카드 선택 막음
			ServerRPC_SelectCard(SelectedCard);
		}
	}	
}

void ANetworkPawn::ServerRPC_SelectCard_Implementation(APawnCard* SelectedCard)
{
	//MulticastRPC_SelectCard(SelectedCard);
	if(Timeline->IsPlaying()) return;
	
	TargetCard = SelectedCard;
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
}

void ANetworkPawn::MulticastRPC_SelectCard_Implementation(APawnCard* SelectedCard)
{
	TargetCard = SelectedCard;
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
	if(HasAuthority())
	{
		//앞면으로 오픈
		Timeline->PlayFromStart();
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

		bool IsSubtract = false;
		if(SelectedCardType == PawnCardType::NoLuck)
		{
			IsSubtract = true;
		}
		
		IncreaseScore(IsSubtract);
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

	/* Turn UI */
	TurnUI = Cast<UTurnUI>(CreateWidget(GetWorld(), TurnUIFactory));
	if(TurnUI && !TurnUI->IsInViewport())
	{
		TurnUI->AddToViewport();
	}
}

void ANetworkPawn::IncreaseScore(bool IsNoLuck)
{
	if(HasAuthority() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("PawnGame IncreaseScore Func is Error"));		
	}
	//매칭에 성공했으면 자신의 UI에 점수를 올린
	ServerRPC_IncreaseScore(this, IsNoLuck);
}

void ANetworkPawn::ServerRPC_DestroyPawnCard_Implementation(APawnCard* FirstTargetCard, APawnCard* SecondTargetCard)
{
	if(!IsValid(FirstTargetCard) || !IsValid(SecondTargetCard)) return;
	
	GameMode->DeleteCardFromMap(FirstTargetCard);
	GameMode->DeleteCardFromMap(SecondTargetCard);

	//FirstTargetCard->Destroy();
	//SecondTargetCard->Destroy();
	
	MulticastRPT_FractureCard(FirstTargetCard, SecondTargetCard);

	// 남은 카드 체크
	GameMode->CheckRemainCards();
}

void ANetworkPawn::MulticastRPC_GameEnd_Implementation(ANetworkPawn* WinnerPlayer)
{
	ANetworkPawn* LocalNetPawn = Cast<ANetworkPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(WinnerPlayer == LocalNetPawn)
	{
		LocalNetPawn->PlayerUI->ShowWinPlayer();
	}
	else
	{
		LocalNetPawn->PlayerUI->ShowLosePlayer();
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

		//첫 번째 선택한 카드도 뒷면으로
		if(FirstSelectedCard.IsValid() && FirstSelectedCard.Get()->FrontBackState == CardState::Front)
		{
			FirstSelectedCard.Get()->SetActorRotation(FRotator(0, FMath::Lerp(TargetCard->GetActorRotation().Yaw, BackRotation.Yaw, value),0));
		}
	}
	else
	{
		//앞면으로
		TargetCard->SetActorRotation(FRotator(0, FMath::Lerp(TargetCard->GetActorRotation().Yaw, FrontRotation.Yaw, value),0));
	}
}

void ANetworkPawn::EndTurnLerp()
{
	TargetCard->ChangeFrontBackState();
	
	if(SecondSelectedCard.IsValid())
	{
		// 매칭 성공 여부
		if(IsCheckCardMatch())
		{
			// 매칭에 성공했으면 PawnCard 삭제
			ServerRPC_DestroyPawnCard(FirstSelectedCard.Get(), SecondSelectedCard.Get());
		}
		else
		{
			SetIsTurnPlayer(false);
			// 잠시 확인
			FTimerHandle TermTimerHandle;
			GetWorldTimerManager().SetTimer(TermTimerHandle, [this]()
			{
				// 실패했으면 선택한 카드 원래대로
				this->Timeline->PlayFromStart();
				//Timeline->Reverse();

				// 턴 교대 로직 시작
				if(this->HasAuthority())
				{
					this->ServerRPC_ChangeTurn(this);
				}
			}, 0.3f, false);
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

	//FString IsUI = (TargetPawn->PlayerUI ? TEXT("Has UI") : TEXT("No UI"));
	FString IsUI = (TargetPawn->TurnUI ? TEXT("Has UI") : TEXT("No UI"));

	FString IsTurn = (TargetPawn->GetIsTurnPlayer() ? TEXT("Turn") : TEXT("No Turn"));

	const UEnum* EnumPtr = StaticEnum<EPieceColor>();
	FString EnumName = EnumPtr->GetNameByValue(static_cast<int64>(PawnPieceColor)).ToString();

	UE_LOG(LogTemp, Warning, TEXT("CheckLog -\nConnection : %s\nOwner : %s\nRole : %s\nAuthority : %s\nIsMine : %s\nPlayerUI : %s\nIsTurn : %s\nColor : %s"),
		*ConnStr, *OwnerStr, *role, *bIsAuthority, *IsMine, *IsUI, *IsTurn, *EnumName)
}

void ANetworkPawn::ChangePlayerTurn(ANetworkPawn* StartPlayer)
{
	MulticastRPC_ChangePlayerTurn(StartPlayer);
}

void ANetworkPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(HasAuthority())
	{
		APawnCardGameMode* gm = Cast<APawnCardGameMode>(GetWorld()->GetAuthGameMode());
		if(!GameMode) GameMode = gm;
		if(gm)
		{
			gm->AddPlayer(this);
		}

		// 서버는 무조건 White
		if(IsLocallyControlled())
		{
			PawnPieceColor = EPieceColor::White;	
		}
		else
		{
			PawnPieceColor = EPieceColor::Black;		
		}
	}
}

void ANetworkPawn::MulticastRPT_FractureCard_Implementation(APawnCard* FirstTargetCard, APawnCard* SecondTargetCard)
{
	FirstTargetCard->MatchingSuccess();
	SecondTargetCard->MatchingSuccess();
	
	FirstTargetCard->StartPhyicsSimul();
	SecondTargetCard->StartPhyicsSimul();

	// 꽝 카드면 데칼 그리기 X
	if(FirstTargetCard->PawnCardData->PawnCardType != PawnCardType::NoLuck && SecondTargetCard->PawnCardData->PawnCardType != PawnCardType::NoLuck)
	{
		DrawDecalActor(FirstTargetCard->GetActorLocation(), PawnPieceColor);
		DrawDecalActor(SecondTargetCard->GetActorLocation(), PawnPieceColor);	
	}

	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;
}

void ANetworkPawn::MulticastRPC_ChangePlayerTurn_Implementation(ANetworkPawn* StartPlayer)
{
	// 다음 턴 플레이어가 로컬의 첫 번째 플레이어와 같으면 자신 턴
	EPieceColor TurnPlayerColor = StartPlayer->PawnPieceColor;
	if(StartPlayer->IsLocallyControlled())
	{
		//초기화
		StartPlayer->PlayerUI->HideTurnStart();
		StartPlayer->PlayerUI->HideEnmTurnStart();

		//My Turn UI
		StartPlayer->PlayerUI->ShowTurnStart();
		
		/*Turn UI */
		StartPlayer->TurnUI->ShowTurn(TurnPlayerColor);
	}
	else
	{
		ANetworkPawn* LocalNetPawn = Cast<ANetworkPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

		//초기화
		LocalNetPawn->PlayerUI->HideTurnStart();
		LocalNetPawn->PlayerUI->HideEnmTurnStart();

		//Enemy Turn UI
		LocalNetPawn->PlayerUI->ShowEnmTurnStart();
		
		/*Turn UI */
		LocalNetPawn->TurnUI->ShowTurn(TurnPlayerColor);
	}
}

void ANetworkPawn::SetIsTurnPlayer(bool IsTurn)
{
	IsTurnPlayer = IsTurn;
}

bool ANetworkPawn::GetIsTurnPlayer()
{
	return IsTurnPlayer;
}

// SetScore를 통해 점수 증가
void ANetworkPawn::ServerRPC_IncreaseScore_Implementation(ANetworkPawn* ScorePlayer, bool IsNoLuck)
{
	// 꽝 카드를 매칭시켰으면 턴 체인
	if(IsNoLuck)
	{
		//CurrentScore--;
		ServerRPC_ChangeTurn(this);
		return;
	}
	
	int32 CurrentScore = ScorePlayer->GetPlayerState()->GetScore();
	CurrentScore++;
	
	ScorePlayer->GetPlayerState()->SetScore(CurrentScore);

	MulticastRPC_IncreaseScore(ScorePlayer, CurrentScore);
}

// UI의 점수 세팅 (PlayerState를 통한 Score 동기화가 느려서 매개변수로 처리)
void ANetworkPawn::MulticastRPC_IncreaseScore_Implementation(ANetworkPawn* ScorePlayer, int32 Score)
{
	// 득점자가 자신이면 MyScoreUI 증가
	if(ScorePlayer->IsLocallyControlled())
	{
		if(ScorePlayer->PlayerUI)
		{
			ScorePlayer->PlayerUI->SetMyScore(Score);
		}
	}
	else
	{
		// 득점자가 자신이 아니면 PlayerController를 가져와서 EnemyScoreUI 증가
		if(ANetworkPawn* ServerPawn = Cast<ANetworkPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			if(ServerPawn->PlayerUI)
			{
				ServerPawn->PlayerUI->SetEnemyScore(Score);	
			}
		}
	}
}