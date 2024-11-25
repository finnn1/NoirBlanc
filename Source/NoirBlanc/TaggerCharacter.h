// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "UIUpdatable.h"
#include "GameFramework/Character.h"
#include "TaggerCharacter.generated.h"

UCLASS()
class NOIRBLANC_API ATaggerCharacter : public ACharacter, public IUIUpdatable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATaggerCharacter();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* TaggerMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UTypingUIWidget> TypingUIWidgetClass;
	class UTypingUIWidget* TypingUIWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UTaggerUIWidget> TaggerUIWidgetClass;
	class UTaggerUIWidget* TaggerUIWidget;

	
	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_SetRandomText() override;

	virtual void UpdateText(const FText& InputedText) override;
	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_UpdateText(const FText& InputedText) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SpawnWeapon(FVector Location, FRotator Rotation, UClass* WeaponClass) override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_SetUITextTo(const FText& InputedText, const FText& CurrentTextToType, const TArray<bool>& StringCorrectArray) override;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Look(const FInputActionValue& Value);
	void JumpStarted(const FInputActionValue& Value);
	void JumpEnded(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpStarted(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_JumpEnded(const FInputActionValue& Value);
	FTimerHandle JumpChargineTimerHandle;
	void ChargeJumpPower();
	float CurrentJumpPower = 0.f;
	UPROPERTY(EditAnywhere, Category = "Tagger Jump")
	float MultiplyJumpPower = 150.f;
	UPROPERTY(EditAnywhere, Category = "Tagger Jump")
	float MinJumpPower = 100.f;
	UPROPERTY(EditAnywhere, Category = "Tagger Jump")
	float MaxJumpPower = 1000.f;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateProgressBar(float Value);
	
	// 점프 힘에 따라 스케일 줄어들기
	void UpdateScale();
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"), Category = "Tagger Jump")
	float MinSquashScale = 0.3f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"), Category = "Tagger Jump")
	float MaxSquashScale = 1.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
