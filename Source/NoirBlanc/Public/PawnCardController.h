// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PawnCardController.generated.h"

class UControllerUI;
/**
 * 
 */
class UPlayerUI;
UCLASS()
class NOIRBLANC_API APawnCardController : public APlayerController
{
	GENERATED_BODY()

public:
	APawnCardController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UControllerUI> CntrUIFactory;
	
	UPROPERTY()
	UControllerUI* CntrUI;
	
};
