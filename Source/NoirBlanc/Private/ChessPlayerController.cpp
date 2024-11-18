// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void AChessPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AActor* AChessPlayerController::TraceForActor()
{
	// 마우스 위치 가져오기
	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY))
	{
		return nullptr;
	}

	// 마우스 위치를 월드 좌표로 변환
	FVector WorldLocation, WorldDirection;
	if (DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
	{
		// 라인 트레이스 시작 위치와 끝 위치 계산
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f); // 10,000 units 거리만큼 라인 트레이스

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetPawn()); // 자신을 무시하도록 설정

		// 라인 트레이스 수행
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			Start, 
			End, 
			ECC_Visibility, // 충돌 채널 설정
			Params
		);

		// 디버그 라인 그리기 (선택 사항)
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);

		// 충돌된 액터가 있을 경우
		if (bHit && HitResult.GetActor())
		{
			AActor* HitActor = HitResult.GetActor();

			// 원하는 동작 수행 (예: 액터 선택, 하이라이트 등)
			return HitActor;
		}
	}
	return nullptr;
}