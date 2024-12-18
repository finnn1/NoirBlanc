// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


// two types: raycast (linetrace in ue) and projectile
// the projectile get influence by gravity and velocity 

UCLASS()
class NOIRBLANC_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// class  UCapsuleComponent* 
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Grenade;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere)
	FVector WindForce;

	UPROPERTY(EditAnywhere)
	float WindResistance;

	UPROPERTY(EditAnywhere)
	float LaunchSpeed;

	UPROPERTY(EditAnywhere)
	FVector LaunchDirection;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
