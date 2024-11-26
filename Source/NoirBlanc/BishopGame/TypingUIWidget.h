// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TypingUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UTypingUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* TextToTypeUI;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TypedTextUI;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* UserInput;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTextChanged(const FText& Text);

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type InCommitType);

public:
	void SetStyledTextToTypeUI(FText TextToType, TArray<bool> StringCorrectArray);
	
private:
	bool bPasteDetected = false;
	
	FString ToCorrectText(TCHAR Character);
	FString ToWrongText(TCHAR Character);
};
