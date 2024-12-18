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

	/*ConstructorHelpers::FClassFinder<AFieldSystemActor> FieldClass(TEXT("/Engine/EditorResources/FieldNodes/FS_MasterField.FS_MasterField_C"));
	if(FieldClass.Succeeded())
	{
		MasterFieldActor = FieldClass.Class;
	}*/
	
	/*GeometryCollectionComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Shatter StaticMesh"));
	GeometryCollectionComp->SetupAttachment(StaticMeshComp);
	GeometryCollectionComp->SetRelativeLocation(FVector(0, 20, 0));
	GeometryCollectionComp->SetSimulatePhysics(false);
	GeometryCollectionComp->SetVisibility(false);
	GeometryCollectionComp->SetEnableGravity(false);*/
	
	bIsSelectable = false;
	bReplicates = true;

	FrontBackState = CardState::Back;

	// Shake
	ShakeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartShakeFloat.BindUFunction(this, FName("StartShakeFunc"));
	EndShakeEvent.BindUFunction(this, FName("EndShakeFunc"));
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

	if (ShakingCurve)
	{
		ShakeTimeline->AddInterpFloat(ShakingCurve, StartShakeFloat);
		ShakeTimeline->SetTimelineFinishedFunc(EndShakeEvent);
		
		ShakeTimeline->SetLooping(false);
		ShakeTimeline->SetTimelineLength(0.2f);
	}
}

void APawnCard::StartShakeFunc(float value)
{
	FVector ChangeLocation = GetActorLocation();
	if(value >= 0.5)
	{
		ChangeLocation.X = FMath::Lerp(GetActorLocation().X, GetActorLocation().X-0.5f, value);
	}
	else
	{
		ChangeLocation.X = FMath::Lerp(GetActorLocation().X, GetActorLocation().X+0.5f, value);
	}
	SetActorLocation(ChangeLocation);
}

void APawnCard::EndShakeFunc()
{
	SetActorLocation(OriginLocation);
}

void APawnCard::StartLerpMaterial()
{
	if(!PawnCardData) return;
	GetWorldTimerManager().SetTimer(LerpTimer, [this]()
	{
		if(!StaticMeshComp) return;
		CurrentLerpTime += LerpCycle;

		if(CurrentLerpTime >= 1)
		{
			StaticMeshComp->SetVisibility(false, false);
			//StartPhyicsSimul();
			GetWorldTimerManager().ClearTimer(LerpTimer);
			return;
		}
		
		PawnCardData->SetMatchingMat_ChaosDest(StaticMeshComp, CurrentLerpTime);	
		
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
	DissolvePawnCardMat();
	/*if(PawnCardData)
	{
		return PawnCardData->Score;
	}*/
	
	return 1;
}

void APawnCard::CancelMatching()
{
	bIsSelectable = true;
	OwnerPlayerState = nullptr;
}


// 블루프린트에서 호출
void APawnCard::BroadcastEndDestruction()
{
	OnFinishSetMat.Broadcast(this);
}

void APawnCard::DissolvePawnCardMat()
{
	CurrentLerpTime = -0.1;
	if(!PawnCardData) return;
	GetWorldTimerManager().SetTimer(LerpTimer, [this]()
	{
		if(!StaticMeshComp) return;
		CurrentLerpTime += LerpCycle;

		if(CurrentLerpTime >= 0.7)
		{
			GetWorldTimerManager().ClearTimer(LerpTimer);
			OnFinishSetMat.Broadcast(this);
			return;
		}
		
		PawnCardData->SetMatchingMat_Dissolve(StaticMeshComp, CurrentLerpTime);	
		
	}, LerpCycle, true);	
}

void APawnCard::StartShakeTimeline()
{
	if(!ShakeTimeline->IsPlaying())
	{
		OriginLocation = GetActorLocation();
		ShakeTimeline->PlayFromStart();	
	}
}
