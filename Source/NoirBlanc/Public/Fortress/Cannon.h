// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.generated.h"

class AProjectileEqBased;
class UInputAction;
struct FInputActionValue;
class UStaticMeshComponent;
class AProjectile;

UCLASS()
class NOIRBLANC_API ACannon : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACannon();

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Muzzle;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SpawnOrigin;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SpawnLocation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ProjectileDirAction;

	UPROPERTY(EditAnywhere)
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere)
	UInputAction* ForceAction;

	void Move(const FInputActionValue& Value);

	void ProjectileDir(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);
	
	void Force(const FInputActionValue& Value);

	FVector MovementInput;
	FRotator RotationInput;

public:
	float MoveSpeed;
	float RotationSpeed;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<AProjectile> ProjectileFactory;		// class
	//
	// UPROPERTY(EditAnywhere)
	// AProjectile* Projectile;	// object

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileEqBased> ProjectileEqBasedFactory;		// class

	UPROPERTY(EditAnywhere)
	AProjectileEqBased* ProjectileEqBased;	// object

};
