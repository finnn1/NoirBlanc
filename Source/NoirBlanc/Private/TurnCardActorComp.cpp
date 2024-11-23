// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnCardActorComp.h"

// Sets default values for this component's properties
UTurnCardActorComp::UTurnCardActorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartTurnFloat.BindUFunction(this, FName("StartTurnLerp"));
	EndTurnEvent.BindUFunction(this, FName("EndTurnLerp"));

	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;

	FrontRotation = FRotator(0,180,0);
	BackRotation = FRotator::ZeroRotator;
}


// Called when the game starts
void UTurnCardActorComp::BeginPlay()
{
	Super::BeginPlay();

	if (MovingCurve)
	{
		Timeline->AddInterpFloat(MovingCurve, StartTurnFloat);
		Timeline->SetTimelineFinishedFunc(EndTurnEvent);
		
		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(0.3f);
	}
	
}


// Called every frame
void UTurnCardActorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTurnCardActorComp::TurnSelectCard(APawnCard* TargetCard)
{
	if(FirstSelectedCard.IsValid())
	{
		//이미 첫 번째 카드를 선택했으면 체크
		SecondSelectedCard = TargetCard;
		UE_LOG(LogTemp, Warning, TEXT("세컨드로"));
	}
	else
	{
		//아무 것도 선택 안 했으면 FirstSelectCard에 할당
		FirstSelectedCard = TargetCard;
		UE_LOG(LogTemp, Warning, TEXT("퍼스트로"));
	}
	
	Timeline->PlayFromStart();
	TargetCard->Selected();
}

void UTurnCardActorComp::RollbackCard(APawnCard* FirstSel, APawnCard* SecondSel)
{
	/*FirstSelectedCard = FirstSel;
	SecondSelectedCard = SecondSel;
	UE_LOG(LogTemp, Warning, TEXT("Fist is %s, %d\n second is %s, %d"), *FirstSel->GetName(), FirstSel->FrontBackState, *SecondSel->GetName(), SecondSel->FrontBackState);*/
	CardsReturnToBack.Empty();
	
	FirstSelectedCard = nullptr;
	SecondSelectedCard = nullptr;
	
	CardsReturnToBack.Add(FirstSel);
	CardsReturnToBack.Add(SecondSel);
	
	Timeline->PlayFromStart();

	//FirstSel->ChangeFrontBackState();
	//SecondSel->ChangeFrontBackState();
}


void UTurnCardActorComp::StartTurnLerp(float value)
{
	//매칭 실패한 카드들 제자리로
	if(CardsReturnToBack.Num() > 0)
	{
		for(APawnCard* Card : CardsReturnToBack)
		{
			Card->SetActorRotation(FRotator(0, FMath::Lerp(Card->GetActorRotation().Yaw, BackRotation.Yaw, value),0));	
		}
	}
	
	if(!FirstSelectedCard.IsValid()) return;
	
	if(FirstSelectedCard.Get()->FrontBackState == CardState::Front)
	{
		//앞면으로
		if(SecondSelectedCard.IsValid() && SecondSelectedCard.Get()->FrontBackState == CardState::Back)
		{
			//UE_LOG(LogTemp, Warning, TEXT("세컨드 앞으로"));
			SecondSelectedCard.Get()->SetActorRotation(FRotator(0, FMath::Lerp(SecondSelectedCard.Get()->GetActorRotation().Yaw, FrontRotation.Yaw, value),0));
		}
		
		if(SecondSelectedCard.IsValid() && SecondSelectedCard.Get()->FrontBackState == CardState::Front)
		{
			//UE_LOG(LogTemp, Warning, TEXT("세컨드 뒤로"));
			SecondSelectedCard.Get()->SetActorRotation(FRotator(0, FMath::Lerp(SecondSelectedCard.Get()->GetActorRotation().Yaw, BackRotation.Yaw, value),0));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("퍼스트 앞으로"));
		FirstSelectedCard.Get()->SetActorRotation(FRotator(0, FMath::Lerp(FirstSelectedCard.Get()->GetActorRotation().Yaw, FrontRotation.Yaw, value),0));
	}
}

void UTurnCardActorComp::EndTurnLerp()
{
	if(SecondSelectedCard.IsValid())
	{
		SecondSelectedCard.Get()->ChangeFrontBackState();
		//UE_LOG(LogTemp, Warning, TEXT("second is %s, %d"), *SecondSelectedCard->GetName(), SecondSelectedCard->FrontBackState);

		if(FirstSelectedCard.IsValid() && FirstSelectedCard.Get()->FrontBackState == CardState::Front && SecondSelectedCard.Get()->FrontBackState == CardState::Front)
		{
			OnFinishCardTurn.Broadcast(FirstSelectedCard.Get(), SecondSelectedCard.Get());
		}
	}
	else if(FirstSelectedCard.IsValid())
	{
		FirstSelectedCard.Get()->ChangeFrontBackState();
		//UE_LOG(LogTemp, Warning, TEXT("Fist is %s, %d"), *FirstSelectedCard->GetName(), FirstSelectedCard->FrontBackState);
	}
	
}

