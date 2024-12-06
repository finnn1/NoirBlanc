// Fill out your copyright notice in the Description page of Project Settings.


#include "Dancing.h"

// Sets default values
ADancing::ADancing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADancing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADancing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Angle = FMath::Sin(GetWorld()->GetTimeSeconds() * Frequency) * Amplitude;
	FRotator NewRotation = FRotator(0.0f, Angle, 0.0f); // Roll을 좌우로 기울이려면 첫 번째 파라미터에 적용
	SetActorRotation(NewRotation);

}

