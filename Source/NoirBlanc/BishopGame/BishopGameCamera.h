// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BishopGameCamera.generated.h"

UCLASS()
class NOIRBLANC_API ABishopGameCamera : public AActor
{
	GENERATED_BODY()

public:
	ABishopGameCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	AActor* Player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	AActor* Player2;

	// 현재 카메라 거리를 저장하는 변수 추가
	UPROPERTY(VisibleAnywhere)
	float CurrentCameraDistance;

	// 카메라 거리 관련 설정값
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinCameraDistance = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxCameraDistance = 12000.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float DistanceMultiplier = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLerpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraHeight = 200.0f;

	void UpdateCameraPosition(float DeltaTime);
};
