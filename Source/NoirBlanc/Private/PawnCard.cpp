// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCard.h"
#include "PawnCardDataAsset.h"

// Sets default values
APawnCard::APawnCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnCard StaticMesh"));
	SetRootComponent(StaticMeshComp);

	bIsMatched = false;
	bIsSelectable = false;

	FrontBackState = CardState::Back;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartTurnFloat.BindUFunction(this, FName("StartTurnLerp"));
}

// Called when the game starts or when spawned
void APawnCard::BeginPlay()
{
	Super::BeginPlay();

	bIsSelectable = true;

	if(PawnCardData && PawnCardData->CardMaterial)
	{
		if(StaticMeshComp)
		{
			StaticMeshComp->SetMaterial(0, PawnCardData->CardMaterial);
			UE_LOG(LogTemp, Warning, TEXT("Set Material"));
		}
	}

	if (MovingCurve)
	{
		Timeline->AddInterpFloat(MovingCurve, StartTurnFloat);
		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(1.f);
	}
}

void APawnCard::TurnToFront()
{
	UE_LOG(LogTemp, Warning,TEXT("Turn2Front.............."));
	// 현재 뒷면이면 뒤집는다 
	if(FrontBackState == CardState::Back)
	{
		FrontBackState = CardState::Front;
		//SetActorRotation(FRotator(0,0,0));
		Timeline->PlayFromStart();
	}
}

void APawnCard::TurnToBack()
{
	UE_LOG(LogTemp, Warning,TEXT("Turn2Back.............."));
	// 현재 앞면이면 뒤집는다
	if(FrontBackState == CardState::Front)
	{
		FrontBackState = CardState::Back;
		//SetActorRotation(FRotator(180,0,0));
		Timeline->PlayFromStart();
	}
}

void APawnCard::CompletedMatching()
{
	bIsMatched = true;
	bIsSelectable = false;

	//TODO 매칭에 선택한 플레이어 정보를 등록
}

void APawnCard::CancelMatching()
{
	bIsMatched = false;
	bIsSelectable = true;

	OwnerPlayerState = nullptr;
}

void APawnCard::StartTurnLerp(float value)
{
	FRotator FrontRotation = FRotator::ZeroRotator;
	FRotator BackRotation = FRotator(180,0,0);

	if(FrontBackState == CardState::Front)
	{
		//뒷면으로
		SetActorRotation(FMath::Lerp(BackRotation, FrontRotation, value));
	}
	else
	{
		//앞면으로
		SetActorRotation(FMath::Lerp(FrontRotation, BackRotation, value));
	}
}
