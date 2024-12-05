// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TempLobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UTempLobbyUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY()
	class UNoirBlancGameInstance* GameInstance;
	
	// Page 0
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoToCreateSession;
	UFUNCTION()
	void GoToCreateSession();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_FindSession;
	UFUNCTION()
	void FindSession();

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_Sessions;

	// Page 1
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TextBox_SessionName;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxKey* ComboBox_GameList;
	UFUNCTION()
	UWidget* OnGenerateContentWidget(FName ItemText);
	UFUNCTION()
	UWidget* OnGenerateItemWidget(FName ItemText);
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateSession;
	UFUNCTION()
	void CreateSession();

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoBackToSessionList;
	UFUNCTION()
	void GoBackSessionList();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTempLobbySessionItem> SessionItemClass;
	
	UFUNCTION()
	void OnAddSession(int32 Index, FString Info);
	UFUNCTION()
	void OnFindComplete(bool bIsComplete);

};
