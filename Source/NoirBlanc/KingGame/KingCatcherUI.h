// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KingCatcherUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UKingCatcherUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* Panel_Buttons;

	TArray<class UKingCatcherFireButtonUI*> AllSpawnLocationButtons;

	void SetButtonColor(int32 ButtonIndex, bool IsSelected);
protected:
	virtual void NativeConstruct() override;
};
