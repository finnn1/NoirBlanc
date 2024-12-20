// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnLocation.h"
#include "Blueprint/UserWidget.h"
#include "KingCatcherFireButtonUI.generated.h"

UCLASS()
class NOIRBLANC_API UKingCatcherFireButtonUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_FireLocation;
	UFUNCTION()
	void OnButtonClicked();
	
	UPROPERTY(EditAnywhere)
	int32 Index = -1;
	bool bIsSelected = false;

	void SetButtonSelected(bool bSelected);
	
protected:
	virtual void NativeConstruct() override;
};
