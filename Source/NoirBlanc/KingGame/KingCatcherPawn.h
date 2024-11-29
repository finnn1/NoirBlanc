// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KingCatcherPawn.generated.h"

UCLASS()
class NOIRBLANC_API AKingCatcherPawn : public APawn
{
	GENERATED_BODY()

public:
	AKingCatcherPawn();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UKingCatcherUI> CatcherUIClass;
	
	class UKingCatcherUI* CatcherUI;

	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnConfirmButtonClicked();
	
	TArray<class ASpawnLocation*> SelectedSpawnLocations;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CatcherMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ClickAction;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Click(const struct FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_Click(FVector WorldLocation, FVector WorldDirection);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SelectForAll(ASpawnLocation* SpawnLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeselectForAll(ASpawnLocation* SpawnLocation);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SelectOnlyForLocallyPlayer(ASpawnLocation* SpawnLocation);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeselectOnlyForLocallyPlayer(ASpawnLocation* SpawnLocation);
};
