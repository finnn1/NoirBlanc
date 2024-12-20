// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/SpawnLocation.h"

#include "KingCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

ASpawnLocation::ASpawnLocation()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	FrontArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FrontArrow"));
	FrontArrow->SetupAttachment(RootComponent);
}

void ASpawnLocation::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}

	// 동적으로 색을 바꾸기 위한 Material Instance Dynamic 할당
	UMaterialInterface* _OriginalMaterial = StaticMeshComponent->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(_OriginalMaterial, this);

	if (DynamicMaterial)
	{
		StaticMeshComponent->SetMaterial(0, DynamicMaterial);
	}
}

void ASpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnLocation::ColorToRed()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void ASpawnLocation::ColorToWhite()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void ASpawnLocation::Select()
{
	ColorToRed();
}

void ASpawnLocation::Deselect()
{
	ColorToWhite();
}
