// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BishopGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API ABishopGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABishopGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;	

	FText CurrentTextToType;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> AllStartPoints;
	int StartPointOrder = -1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABishopPawn> BishopPawnClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACharacter> TaggerPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> BishopWeaponClass;
	
	bool CheckCommittedText(const FText& TypedText);
	void UpdateInputtedText(const FText& TypedText);
	void CommitText(const FText& TypedText);
	FText PickRandomText();

protected:
	virtual void BeginPlay() override;
	

private:
	UPROPERTY(EditAnywhere)
	TArray<FText> TextsToType;
};
