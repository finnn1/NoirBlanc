// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardController.h"

#include <ObjectArray.h>
#include "PawnCardGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnCard.h"
#include "PawnCardDataAsset.h"
#include "PlayerUI.h"
//#include "TurnCardActorComp.h"

APawnCardController::APawnCardController()
{
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartTurnFloat.BindUFunction(this, FName("StartTurnLerp"));
	EndTurnEvent.BindUFunction(this, FName("EndTurnLerp"));
}

void APawnCardController::BeginPlay()
{
	Super::BeginPlay();
	
	//턴 시작
	GameMode = Cast<APawnCardGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->OnTurnStart.AddDynamic(this, &APawnCardController::InitPlayerSettings);
		GameMode->OnTurnEnd.AddDynamic(this, &APawnCardController::PlayerTurnEnd);
		GameMode->OnChangeScore.AddDynamic(this, &APawnCardController::IncreaseScore);
		
		GameMode->AddPlayerController(this);
	}
	
	if(APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem)
		{
			//우선순위 0으로 설정
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if(IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("추가: %s"), *this->GetName());
		if (MovingCurve)
		{
			Timeline->AddInterpFloat(MovingCurve, StartTurnFloat);
			Timeline->SetTimelineFinishedFunc(EndTurnEvent);
		
			Timeline->SetLooping(false);
			Timeline->SetTimelineLength(0.3f);
		}

		InitPlayerUI();
	}

	if(HasAuthority())
	{
		GameMode->InitPawnCardGame();
	}
}

void APawnCardController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputCompo = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputCompo->BindAction(CardClickActioin, ETriggerEvent::Started, this, &APawnCardController::SelectCard);
	}
}

void APawnCardController::InitPlayerSettings()
{
	if(!GetTurnOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("Start is %s, %d"), *this->GetName(), GetTurnOwner());
	
	//마우스 커서 보이게 하기
	bShowMouseCursor = true;

	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;

	if(PlayerUI)
	{
		PlayerUI->ShowTurnStart();
	}
}

void APawnCardController::SelectCard(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Selected"));
	if(Timeline->IsPlaying()) return;
	if(!GetTurnOwner()) return;
	
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Selected Anything"));
	}
}

bool APawnCardController::IsCheckCardMatch(/*APawnCard* FirstSelectedCard, APawnCard* SecondSelectedCard*/)
{
	/*UE_LOG(LogTemp, Warning, TEXT("%s and %s"), *FirstSelectedCard->GetName(), *SecondSelectedCard->GetName());
	if(!FirstSelectedCard || !SecondSelectedCard) return false;*/

	bool bIsMatch = false;
	
	PawnCardType SelectedCardType = FirstSelectedCard.Get()->PawnCardData->PawnCardType;
	PawnCardType TargetCardType = SecondSelectedCard.Get()->PawnCardData->PawnCardType;
	
	if(SelectedCardType == TargetCardType)
	{
		bIsMatch = true;
		
		FirstSelectedCard->SuccessMatching(this);
		SecondSelectedCard->SuccessMatching(this);
		
		FirstSelectedCard->Destroy();
		SecondSelectedCard->Destroy();

		MulticastRPC_IncreaseScore();
	}

	return bIsMatch;
}

void APawnCardController::InitPlayerUI()
{
	PlayerUI = Cast<UPlayerUI>(CreateWidget(GetWorld(), TSubPlayerUI));

	if(PlayerUI)
	{
		PlayerUI->AddToViewport();

		PlayerUI->HideTurnStart();
		PlayerUI->HideTurnEnd();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InitPlayerUI is Failed"));
	}
}


void APawnCardController::StartTurnLerp(float value)
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

void APawnCardController::EndTurnLerp()
{
	TargetCard->ChangeFrontBackState();
	if(SecondSelectedCard.IsValid())
	{
		if(IsCheckCardMatch())
		{
			PlayerScore += 2;
		}
		else
		{
			Timeline->PlayFromStart();
			if(GameMode)
			{
				GameMode->TurnEnd(this);
			}
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


void APawnCardController::PlayerTurnEnd()
{
	if(GetTurnOwner()) return;
	
	if(PlayerUI)
	{
		PlayerUI->ShowTurnEnd();
	}

	bShowMouseCursor = false;
}


void APawnCardController::SetTurnOwner(bool IsOwner)
{
	IsTurnOwner = IsOwner;
}

bool APawnCardController::GetTurnOwner()
{
	return IsTurnOwner;
}

void APawnCardController::MulticastRPC_IncreaseScore_Implementation()
{
	if(GameMode)
	{
		GameMode->ChangeScore();
	}
}

void APawnCardController::IncreaseScore()
{
	if(!PlayerUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("No name is %s"), *this->GetName());
		return;
	}
	if(GetTurnOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("My name is %s"), *this->GetName());
		PlayerUI->IncreaseScore();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy name is %s"), *this->GetName());
		PlayerUI->IncreaseEnemyScore();
	}
}
