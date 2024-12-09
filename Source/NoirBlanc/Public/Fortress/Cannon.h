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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* VelocityBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFireBoostWidget* FireBoostWidget;
	
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
	UInputAction* FireBoostAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ForceAction;

	void Move(const FInputActionValue& Value);

	void ProjectileDir(const FInputActionValue& Value);

	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire(float Velocity, TSubclassOf<AProjectileEqBased> ProjectileEqBasedSubclass);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Fire(float Velocity, TSubclassOf<AProjectileEqBased> ProjectileEqBasedSubclass);
	
	void StartCharging(const FInputActionValue& Value);

	void ContinueCharging();
	
	void Force(const FInputActionValue& Value);

	FVector MovementInput;
	FRotator RotationInput;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Move(FVector NewLocation, FRotator NewRotation);
	
	void BillboardFireBoost();
	
public:
	float MoveSpeed;
	float RotationSpeed;

	// timer for setting projectile velocity
	FTimerHandle SpeedIncreaseTimerHandle;

	UPROPERTY(EditAnywhere)
	float ProjectileVelocity;
	
	UPROPERTY(EditAnywhere)
	float VelocityChange = 100.0f;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

public:
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<AProjectile> ProjectileFactory;		// class
	//
	// UPROPERTY(EditAnywhere)
	// AProjectile* Projectile;	// object

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileEqBased> ProjectileEqBasedFactory;		// class

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACannonBall> CannonBallFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMissile> MissileFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABomb> BombFactory;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AProjectileEqBased>>ProjectilesEqBasedFactoryArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileEqBased> ProjectileEqBasedClass;	// object

	TArray<int32> randomProjectileIdxArray;
	int32 numprojectile;
	int32 projectileIdx = 0;
	
public:
	// Health and Damage
	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)

	float Health;

	// UPROPERTY(EditAnywhere)
	// float Damage;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> FortressUIFactory;
	class UFortressUI* FortressUI;
	
	void InitMainUIWiget();
	UFUNCTION(Client, Reliable)
	void ClientRPC_Init();

	UPROPERTY(Replicated, VisibleAnywhere)
	FText turnCannon;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bIsturn;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


