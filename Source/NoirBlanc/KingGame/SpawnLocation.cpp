// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/SpawnLocation.h"

#include "KingCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NoirBlanc/BishopGame/TaggerCharacter.h"

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

void ASpawnLocation::SpawnClickSound()
{
	MulticastRPC_SpawnSoundAtLocation(SoundClick, GetActorLocation(), KingGameSoundEffect::Click);
}

void ASpawnLocation::MulticastRPC_SpawnSoundAtLocation_Implementation(USoundBase* Sound, FVector Location, KingGameSoundEffect SoundEffect)
{
	UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, Location);
	if (AudioComp)
	{
		AudioComp->bAutoDestroy = true;
		AudioComp->Play();
	}

	switch (SoundEffect)
	{
	case KingGameSoundEffect::Click:
		AudioComponentClick = AudioComp;
		break;
	case KingGameSoundEffect::ClickDeny:
		AudioComponentClickDeny = AudioComp;
		break;
	default:
		break;
	}
}

void ASpawnLocation::MulticastRPC_StopSound_Implementation(KingGameSoundEffect SoundEffect)
{
	switch (SoundEffect)
	{
	case KingGameSoundEffect::Click:
		if (AudioComponentClick)
		{
			AudioComponentClick->Stop();
		}
		break;
	case KingGameSoundEffect::ClickDeny:
		if (AudioComponentClickDeny)
		{
			AudioComponentClickDeny->Stop();
		}
		break;
	default:
		break;
	}
}

/*void ASpawnLocation::ColorToRed()
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
}*/

void ASpawnLocation::ColorTo_Implementation(FLinearColor color, APlayerController* WhoWantToSee)
{
	if (WhoWantToSee == nullptr)
	{
		if (DynamicMaterial)
		{
			//DynamicMaterial->SetVectorParameterValue("BaseColor", color);

			if (color == FLinearColor::Red)
			{
				StaticMeshComponent->SetMaterial(0, ChooseMat);
			}
			else
			{
				StaticMeshComponent->SetMaterial(0, NorMalMat);
			}
		}
	}
	else
	{
		if (GetWorld()->GetFirstPlayerController() == WhoWantToSee)
		{
			if (DynamicMaterial)
			{
				//DynamicMaterial->SetVectorParameterValue("BaseColor", color);

				if (color == FLinearColor::Red)
				{
					StaticMeshComponent->SetMaterial(0, ChooseMat);
				}
				else
				{
					StaticMeshComponent->SetMaterial(0, NorMalMat);
				}
			}
		}
	}
}
