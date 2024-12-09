// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QueenSelectWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FButtonDelegate, const FString&);

UCLASS()
class NOIRBLANC_API UQueenSelectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn_Pawn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Knight;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Rook;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Btn_Bishop;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnPawnClicked();
	UFUNCTION()
	void OnKnightClicked();
	UFUNCTION()
	void OnRookClicked();
	UFUNCTION()
	void OnBishopClicked();
	
	FButtonDelegate OnBtnClicked;
private:
	FString SelectedLevel = TEXT("Pawn");
};
