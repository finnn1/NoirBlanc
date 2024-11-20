// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardController.h"

#include <ObjectArray.h>
#include "PawnCardGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnCard.h"
#include "PawnCardDataAsset.h"

APawnCardController::APawnCardController()
{
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartTurnFloat.BindUFunction(this, FName("StartTurnLerp"));
	EndTurnEvent.BindUFunction(this, FName("EndTurnLerp"));
}

void APawnCardController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	//턴 시작
	GameMode = Cast<APawnCardGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		GameMode->OnTurnStart.AddDynamic(this, &APawnCardController::InitPlayerSettings);
		GameMode->OnTurnEnd.AddDynamic(this, &APawnCardController::InitPlayerSettings);
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

	if (MovingCurve)
	{
		Timeline->AddInterpFloat(MovingCurve, StartTurnFloat);
		Timeline->SetTimelineFinishedFunc(EndTurnEvent);
		
		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(0.3f);
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
	//마우스 커서 보이게 하기
	bShowMouseCursor = true;

	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;
}

void APawnCardController::SelectCard(const FInputActionValue& Value)
{
	if(Timeline->IsPlaying()) return;
	
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if(Hit.bBlockingHit)
	{
		TargetCard = Cast<APawnCard>(Hit.GetActor());
		if(TargetCard)
		{
			if(FirstSelectedCard.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("FirstSelectedCard Is Valid"));
				//이미 첫 번째 카드를 선택했으면 체크
				SecondSelectedCard = TargetCard;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FirstSelectedCard Not Valid"));
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

bool APawnCardController::IsCheckCardMatch()
{
	if(!FirstSelectedCard.IsValid() || !SecondSelectedCard.IsValid()) return false;

	bool bIsMatch = false;
	
	PawnCardType SelectedCardType = FirstSelectedCard.Get()->PawnCardData->PawnCardType;
	PawnCardType TargetCardType = SecondSelectedCard.Get()->PawnCardData->PawnCardType;

	UE_LOG(LogTemp, Warning, TEXT("Matching PawnCardType: basic is %d, target is %d")
			, SelectedCardType, TargetCardType);
	
	if(SelectedCardType == TargetCardType)
	{
		bIsMatch = true;
		
		FirstSelectedCard.Get()->SuccessMatching(this);
		SecondSelectedCard.Get()->SuccessMatching(this);
		
		FirstSelectedCard.Get()->Destroy();
		SecondSelectedCard.Get()->Destroy();
		
		UE_LOG(LogTemp, Warning, TEXT("Matching Success!!!!!"));
	}
	
	return bIsMatch;
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
			UE_LOG(LogTemp, Warning, TEXT("턴은 끝"));
			//TurnEnd();
		}
	}
	else
	{
		// 첫 번째 선택 카드 초기화
		if(FirstSelectedCard.IsValid())
		{
			if(FMath::IsNearlyEqual(FirstSelectedCard.Get()->GetActorRotation().Yaw, 0, 0.1))
			{
				FirstSelectedCard = nullptr;
			}
		}
	}
}

void APawnCardController::ReturnCardBack(APawnCard* PawnCard)
{
	if(PawnCard && !PawnCard->GetOwnerPlayer())
	{
		TargetCard = PawnCard;
		Timeline->PlayFromStart();
	}
}

void APawnCardController::TurnEnd()
{
	if(GameMode)
	{
		bShowMouseCursor = false;
		GameMode->TurnEnd();
	}
}


void APawnCardController::SetTurnOwner(bool IsOwner)
{
	IsTurnOwner = IsOwner;
}

bool APawnCardController::GetTurnOwner()
{
	return IsTurnOwner;
}