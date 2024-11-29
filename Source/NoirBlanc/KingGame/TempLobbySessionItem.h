// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TempLobbySessionItem.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UTempLobbySessionItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;
	UFUNCTION()
	void JoinSession();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SessionName;
	
	int32 SelectIndex;
	
	void SetInfo(int32 Index, FString Info);;
};
