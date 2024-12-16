// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCard.h"
#include "PawnCardDataAsset.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
APawnCard::APawnCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnCard StaticMesh"));
	SetRootComponent(StaticMeshComp);
	// (X=466.000000,Y=6.000000,Z=263.000000)
	GeometryCollectionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Shatter StaticMesh"));
	GeometryCollectionComp->SetupAttachment(StaticMeshComp);
	GeometryCollectionComp->SetRelativeLocation(FVector(0, 20, 0));
	GeometryCollectionComp->SetSimulatePhysics(false);
	GeometryCollectionComp->SetVisibility(false);
	GeometryCollectionComp->SetEnableGravity(false);
	
	bIsSelectable = false;

	FrontBackState = CardState::Back;
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void APawnCard::BeginPlay()
{
	Super::BeginPlay();

	bIsSelectable = true;
	
	if(PawnCardData)
	{
		PawnCardData->InitMeshMaterial(StaticMeshComp);
	}
}

void APawnCard::SetLerpMaterial()
{
	if(!PawnCardData) return;
	GetWorldTimerManager().SetTimer(LerpTimer, [this]()
	{
		if(!StaticMeshComp) return;
		CurrentLerpTime += LerpCycle;
		PawnCardData->SetMatchingMaterial(StaticMeshComp, CurrentLerpTime);
		
		if(CurrentLerpTime > 1)
		{
			GetWorldTimerManager().ClearTimer(LerpTimer);
			StaticMeshComp->SetVisibility(false, false);
			OnFinishSetMat.Broadcast(this);	
		}
	}, LerpCycle, true);
}

void APawnCard::ChangeFrontBackState()
{
	if(FrontBackState == CardState::Front)
	{
		FrontBackState = CardState::Back;
	}
	else
	{
		FrontBackState = CardState::Front;
	}
}

void APawnCard::InitCard()
{
	bIsSelectable = true;
	FrontBackState = CardState::Back;
}

int32 APawnCard::MatchingSuccess()
{
	SetLerpMaterial();
	if(PawnCardData)
	{
		return PawnCardData->Score;
	}
	
	return 1;
}

void APawnCard::CancelMatching()
{
	bIsSelectable = true;
	OwnerPlayerState = nullptr;
}
