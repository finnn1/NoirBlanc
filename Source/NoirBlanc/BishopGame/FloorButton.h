// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorButton.generated.h"

UCLASS()
class NOIRBLANC_API AFloorButton : public AActor
{
	GENERATED_BODY()

public:
	AFloorButton();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ButtonBodyMeshComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ButtonSwitchMeshComponent;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DetectAreaBoxComponent;

	UPROPERTY(EditAnywhere)
	float PressDepth = 15.f;
	FVector OriginalSwitchLocation;

	bool bIsPressing = false;
	UPROPERTY(EditAnywhere)
	float PressSpeed = 100.f;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnDetectAreaBeginOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnDetectAreaEndOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnButtonPressed();
};
