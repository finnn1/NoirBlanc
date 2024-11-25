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
	
	bIsSelectable = false;

	FrontBackState = CardState::Back;
}

// Called when the game starts or when spawned
void APawnCard::BeginPlay()
{
	Super::BeginPlay();

	bIsSelectable = true;

	if(PawnCardData && PawnCardData->CardMaterialIns)
	{
		if(StaticMeshComp)
		{
			UMaterialInstanceDynamic* TestDynamic = UMaterialInstanceDynamic::Create(PawnCardData->CardMaterialIns, this);
			StaticMeshComp->SetMaterial(4, TestDynamic);
			FLinearColor NewUVOffsets(1.f,1.f, PawnCardData->U_Offset,PawnCardData->V_Offset);
			TestDynamic->SetVectorParameterValue(TEXT("UV"), NewUVOffsets);
		}
	}
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

void APawnCard::Selected()
{
	bIsSelectable = false;
}

bool APawnCard::IsCardSelectable()
{
	return bIsSelectable;
}

void APawnCard::SuccessMatching(APlayerController* OwnerPlayer)
{
	if(OwnerPlayer)
	{
		OwnerPlayerState = OwnerPlayer->PlayerState;
	}
}

bool APawnCard::GetOwnerPlayer()
{
	return (OwnerPlayerState != nullptr ? true : false);
}

void APawnCard::CancelMatching()
{
	bIsSelectable = true;
	OwnerPlayerState = nullptr;
}
