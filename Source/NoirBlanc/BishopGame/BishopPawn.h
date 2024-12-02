// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIUpdatable.h"
#include "GameFramework/Pawn.h"
#include "BishopPawn.generated.h"

UCLASS()
class NOIRBLANC_API ABishopPawn : public APawn, public IUIUpdatable
{
	GENERATED_BODY()

public:
	ABishopPawn();
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UTypingUIWidget> TypingUIWidgetClass;
	
	class UTypingUIWidget* TypingUIWidget;
	
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
	virtual void BeginPlay() override;
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_SetRandomText() override;

	virtual void UpdateText(const FText& InputedText) override;
	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_UpdateText(const FText& InputedText) override;

	void CommitText(const FText& TypedText);
	UFUNCTION(Server, Reliable)
	void ServerRPC_CommitText(const FText& TypedText);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SpawnWeapon(FVector Location, FRotator Rotation, UClass* WeaponClass) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetUITextTo(const FText& InputedText, const FText& CurrentTextToType, const TArray<bool>& StringCorrectArray) override;

};
