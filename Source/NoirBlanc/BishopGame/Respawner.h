// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Respawner.generated.h"

UCLASS()
class NOIRBLANC_API ARespawner : public AActor
{
	GENERATED_BODY()

public:
	ARespawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	APlayerStart* TaggerPlayerStart;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	
	UFUNCTION()
	void OnComponentBeginOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
