// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopWeapon.h"

#include "BishopGameMode.h"
#include "TaggerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PawnMovementComponent.h"

class ABishopGameMode;

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

	// 머티리얼 확인
	UMaterialInterface* _FirstMaterial = MeshComponent->GetMaterial(0);
	if (_FirstMaterial)
	{
		// 동적 머티리얼 생성
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(_FirstMaterial, this);

		// 생성된 동적 머티리얼을 메시에 적용
		if (DynamicMaterialInstance)
		{
			MeshComponent->SetMaterial(0, DynamicMaterialInstance);

			// 초기 파라미터 설정 (예: Opacity 값을 0.5로 설정)
			DynamicMaterialInstance->SetScalarParameterValue(FName("OpacityMultiply"), 0.5f);
		}
	}
}

void ABishopWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FRotator CurrentRotation = GetActorRotation();
	// if (CurrentRotation.Pitch >= 80.0f)
	// {
	// 	Destroy();
	// 	return;
	// }
	//
	// // 회전
	// float PitchMoveValue = AttackSpeed * DeltaTime;
	// AddActorWorldRotation(FRotator(-PitchMoveValue, 0, 0));

	FVector CurrentLocation = GetActorLocation();
	// if (CurrentLocation.Pitch >= 80.0f)
	// {
	// 	Destroy();
	// 	return;
	// }

	// 전진
	float LocationMoveValue = AttackSpeed * DeltaTime;
	AddActorWorldOffset(FVector(-AttackSpeed, 0, 0));

	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(FName("OpacityMultiply"), CurrentMaterialOpacity);
		CurrentMaterialOpacity -= (TransparencySpeed * DeltaTime);
	}
}

void ABishopWeapon::OnBoxComponentOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (HasAuthority())
	{
		ABishopGameMode* _BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
		if (_BishopGameMode)
		{
			_BishopGameMode->OnTaggerOverlapped(OtherActor);
		}
	}
}
