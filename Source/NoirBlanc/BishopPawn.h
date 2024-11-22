// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BishopPawn.generated.h"

UCLASS()
class NOIRBLANC_API ABishopPawn : public APawn
{
	GENERATED_BODY()

	// bool bIsBishop = false;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UTypingUIWidget> TypingUIWidgetClass;

	// UPROPERTY(EditAnywhere)
	// TSubclassOf<class AWeapon> BishopWeaponClass;
	
	class UTypingUIWidget* TypingUIWidget;
	
public:
	// Sets default values for this pawn's properties
	ABishopPawn();

	UPROPERTY(EditAnywhere)
	class UTextRenderComponent* DebugText;
	
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* WeaponSpawnPoint;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetRandomText();

	void UpdateText(const FText& InputedText);
	UFUNCTION(Server, Reliable)
	void ServerRPC_UpdateText(const FText& InputedText);

	void CommitText(const FText& TypedText);
	UFUNCTION(Server, Reliable)
	void ServerRPC_CommitText(const FText& TypedText);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnWeapon(FVector Location, FRotator Rotation, UClass* WeaponClass);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetUITextTo(const FText& InputedText, const FText& NewText);

};
