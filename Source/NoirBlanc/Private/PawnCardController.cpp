// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PawnCard.h"
#include "PawnCardDataAsset.h"

APawnCardController::APawnCardController()
{
	
}

void APawnCardController::BeginPlay()
{
	Super::BeginPlay();

	//턴 시작
	StartTurn();
	
	if(APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem)
		{
			//우선순위 0으로 설정
			Subsystem->AddMappingContext(InputMapping, 0);
		}
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

void APawnCardController::StartTurn()
{
	//마우스 커서 보이게 하기
	bShowMouseCursor = true;

	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;
}

void APawnCardController::EndTurn()
{
	bShowMouseCursor = false;

	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;
}

void APawnCardController::SelectCard(const FInputActionValue& Value)
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if(Hit.bBlockingHit)
	{
		APawnCard* TargetCard = Cast<APawnCard>(Hit.GetActor());
		if(TargetCard && TargetCard->bIsSelectable)
		{
			if(FirstSelectedCard.IsValid())
			{
				//이미 첫 번째 카드를 선택했으면 체크
				SecondSelectedCard = TargetCard;

				//앞면으로 오픈
				SecondSelectedCard->TurnToFront();
				
				IsCheckCardMatch(/*FirstSelectedCard->PawnCardData*/);
			}
			else
			{
				//아무 것도 선택 안 했으면 FirstSelectCard에 할당
				FirstSelectedCard = TargetCard;
				
				//앞면으로 오픈
				FirstSelectedCard->TurnToFront();
			}
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

void APawnCardController::IsCheckCardMatch(/*const UPawnCardDataAsset* TargetPawnCardData*/)
{
	SecondSelectedCard->TurnToFront();
	
	PawnCardType SelectedCardType = FirstSelectedCard->PawnCardData->PawnCardType;
	PawnCardType TargetCardType = SecondSelectedCard->PawnCardData->PawnCardType;

	UE_LOG(LogTemp, Warning, TEXT("Matching PawnCardType: basic is %d, target is %d, queen is %d")
			, SelectedCardType, TargetCardType, PawnCardType::Queen);
	
	if(SelectedCardType == PawnCardType::Queen || TargetCardType == PawnCardType::Queen || SelectedCardType == TargetCardType)
	{
		FirstSelectedCard->CompletedMatching();
		SecondSelectedCard->CompletedMatching();
		
		SecondSelectedCard = nullptr;

		UE_LOG(LogTemp, Warning, TEXT("Matching Success!!!!!"));
	}
	else
	{
		SecondSelectedCard->TurnToBack();
		EndTurn();
		
		//TODO 매칭 실패하면 상대에게 턴을 넘김 (Delegate)
	}
}
