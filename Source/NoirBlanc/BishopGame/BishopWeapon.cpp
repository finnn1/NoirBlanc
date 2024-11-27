// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopWeapon.h"
#include "TaggerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ABishopWeapon::ABishopWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);
}

void ABishopWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABishopWeapon::OnBoxComponentOverlap);
}

void ABishopWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	if (CurrentRotation.Pitch >= 80.0f)
	{
		Destroy();
		return;
	}

	float PitchMoveValue = AttackSpeed * DeltaTime;
	AddActorWorldRotation(FRotator(-PitchMoveValue, 0, 0));
}

void ABishopWeapon::OnBoxComponentOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// TODO: 충돌 판정 및 Destroy 서버에서 하도록 하기
	ATaggerCharacter* TaggerCharacter = Cast<ATaggerCharacter>(OtherActor);
	if (TaggerCharacter == nullptr) return;
	if (TaggerCharacter->GetMovementComponent()->IsFalling() == false) return;
	TaggerCharacter->Destroy();

	// TODO: 서버에서 게임 오버 처리 및 UI 띄우기
	// ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	// BishopGameMode
}
