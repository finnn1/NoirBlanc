// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UInfoUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* Btn_Info;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ShowOnHovered();

	UFUNCTION()
	void HideOnUnhovered();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoTextUI>InfoTextUIClass;

	UPROPERTY()
	UInfoTextUI* InfoTextUI;
};
