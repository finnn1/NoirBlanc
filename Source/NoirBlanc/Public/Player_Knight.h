// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputMappingContext.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Knight.generated.h"

UCLASS()
class NOIRBLANC_API APlayer_Knight : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Knight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Move(const FInputActionValue& Value);

	void Jump();
	
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Player;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Ended = false;

	bool Finished = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMainUI> MainUI;
    class UMainUI* Main;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UFinishUI> FinishUI;
	class UFinishUI* Finish;
	
	UPROPERTY(Replicated)
	float Timer = 30.f;
};
