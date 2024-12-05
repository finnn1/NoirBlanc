// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dancing.generated.h"

UCLASS()
class NOIRBLANC_API ADancing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADancing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float Frequency = 0;
	UPROPERTY(EditAnywhere)
	float Amplitude = 0;
};
