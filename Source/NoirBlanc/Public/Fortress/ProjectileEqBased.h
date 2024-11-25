// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileEqBased.generated.h"

UCLASS()
class NOIRBLANC_API AProjectileEqBased : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileEqBased();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector InitVelocity;
	FVector CurrLocation;
	FVector WindForce;
	float Gravity;
	float ElapsedTime;

	UPROPERTY(EditAnywhere)
	float WindResistance;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float InitSpeed;
	
	UPROPERTY(EditAnywhere)
	float Force;
	
	void SetWindResistance(FVector WindDirection, float Resistance);

	UFUNCTION()
	void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void  OnProjectileHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector OtherNormal,
		const FHitResult& Hit
		);

	UPROPERTY(EditAnywhere)
	UParticleSystem* BombEffect;
};
