// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FortressGameMode.h"
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
	class UWidgetComponent* ImpulseBar;

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
	void ServerRPC_Fire(float Impulse, TSubclassOf<AProjectileEqBased> ProjectileEqBasedSubclass);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Fire(float Impulse, TSubclassOf<AProjectileEqBased> ProjectileEqBasedSubclass);
	
	void StartCharging(const FInputActionValue& Value);

	void ContinueCharging();
	
	void Force(const FInputActionValue& Value);

	FVector MovementInput;
	FRotator RotationInput;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Move(FVector NewLocation, FRotator NewRotation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Move(FVector NewLocation, FRotator NewRotation);
	
	void BillboardFireBoost();
	
public:
	float MoveSpeed;
	float RotationSpeed;

	// timer for setting projectile impulse
	FTimerHandle SpeedIncreaseTimerHandle;

	UPROPERTY(EditAnywhere)
	float ProjectileImpulse;
	
	UPROPERTY(EditAnywhere)
	float ImpulseChange = 100.0f;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual  void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

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
	
	UPROPERTY(Replicated, VisibleAnywhere)
	FText turnCannon;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bIsturn;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector WindForce;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetWindForce(FVector NewWindForce, float NewWindForceMax);

public:
	AFortressGameMode* gm;

	UFUNCTION(Server, Reliable)
	void ServerRPC_NewPlayerJoined();

	UFUNCTION(Client, Reliable)
	void ClientRPC_EnableInput(bool enable);
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_AddToViewport();

	UFUNCTION(Client, Reliable)
	void ClientRPC_SwitchWidget(int32 index);
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_UpdateCountdownUI(const FText& text);

public:
	// make a limit for the movement
	float DistanceRange = 200.0f;

	UPROPERTY(VisibleAnywhere)
	FVector StartLocation;

	// limit for the angle of the muzzle
	float AngleRange = 40.0f;

	UPROPERTY(VisibleAnywhere)
	FRotator StartRotation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent*  AngleWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAngleWidget* AngleWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* BackgroundSound;
	
};





