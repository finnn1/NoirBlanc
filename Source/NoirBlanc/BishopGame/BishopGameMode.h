// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BishopGameMode.generated.h"

UENUM(BlueprintType)
enum class EBishopGamePlayerType : uint8
{
	Bishop		UMETA(DisplayName = "Bishop"),
	Tagger		UMETA(DisplayName = "Tagger"),
};

UCLASS()
class NOIRBLANC_API ABishopGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABishopGameMode();

	TArray<APlayerController*> JoinedPlayers;
	
	FText CurrentTextToType;
	FString CurrentTypedText;

	UPROPERTY(EditAnywhere)
	TArray<APlayerStart*> AllStartPoints;
	int StartPointOrder = -1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABishopPawn> BishopPawnClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACharacter> TaggerPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABishopWeapon> BishopWeaponClass;

public:
	FTimerHandle StartCountDownTimerHandle;
	void NotifyJoined(APlayerController* JoinedPlayer);
	void StartCountTimer();
	int32 CountdownNumber = 4;

	FTimerHandle MainTimerHandle;
	void UpdateTimer();
	int32 CurrentRemainTime;
	UPROPERTY(EditAnywhere)
	int32 DefaultRemainTime = 100;
	
	// TODO: 접근 제한자 올바르게 할당
	bool CheckCommittedText(const FText& TypedText);
	void UpdateInputtedText(const FText& TypedText);
	void PickRandomTextAndUpdateUI();
	void CommitText(const FText& TypedText);
	FText PickRandomText();

	void GameOver(APawn* Winner);

	bool CheckCheatting(const FText& TypedText);

	void OnTaggerOverlapped(AActor* OtherActor);
	void OnButtonPressed();
	TArray<bool> CheckTypingCorrect(const FText& TypedText);
	
protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere)
	TArray<FText> TextsToType;
};
