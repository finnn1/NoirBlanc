#include "NoirBlanc/BishopGame/BishopGameCamera.h"
#include "Camera/CameraComponent.h"

ABishopGameCamera::ABishopGameCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SetRootComponent(CameraComponent);
}

void ABishopGameCamera::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void ABishopGameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player1 && Player2)
	{
		UpdateCameraPosition(DeltaTime);
	}
}

void ABishopGameCamera::UpdateCameraPosition(float DeltaTime)
{
	if (!Player1 || !Player2) return;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player1 : %s"), *Player1->GetActorNameOrLabel()));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player2 : %s"), *Player2->GetActorNameOrLabel()));
	//
	//
	// FVector Player1Location = Player1->GetActorLocation();
	// FVector Player2Location = Player2->GetActorLocation();
	//
	// FVector MidPoint = (Player1Location + Player2Location) / 2.0f;
	//
	// FVector CameraLocation = MidPoint + FVector(0.0f, 3500.0f, 300.0f); // 높이 고정
	// SetActorLocation(CameraLocation);

	// FOV
	// float Distance = FVector::Distance(Player1Location, Player2Location);
	// float NewFOV = FMath::Clamp(Distance * 0.2f, 90.0f, 120.0f);
	// CameraComponent->SetFieldOfView(NewFOV);

	
	// 플레이어 간 거리 계산
	float PlayerDistance = FVector::Distance(
		Player1->GetActorLocation(),
		Player2->GetActorLocation()
	);

	// 원하는 카메라 거리 계산
	float TargetDistance = PlayerDistance * DistanceMultiplier;
    
	// 최소/최대 거리 제한
	TargetDistance = FMath::Clamp(
		TargetDistance, 
		MinCameraDistance, 
		MaxCameraDistance
	);

	// 부드러운 카메라 이동을 위한 보간
	CurrentCameraDistance = FMath::FInterpTo(
		CurrentCameraDistance,
		TargetDistance,
		DeltaTime,
		CameraLerpSpeed
	);

	// 두 플레이어의 중간 지점 계산
	FVector MidPoint = (Player1->GetActorLocation() + Player2->GetActorLocation()) * 0.5f;
    
	// 카메라 위치 설정
	FVector CameraLocation = MidPoint;
	CameraLocation.Z += CameraHeight;
	CameraLocation.Y += CurrentCameraDistance;  // 현재 계산된 거리 적용
    
	// 부드러운 카메라 위치 이동
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(
		CurrentLocation,
		CameraLocation,
		DeltaTime,
		CameraLerpSpeed
	);

	SetActorLocation(NewLocation);
}