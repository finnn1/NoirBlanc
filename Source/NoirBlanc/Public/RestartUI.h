// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API URestartUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn_Restart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_End;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnEndClicked();
};