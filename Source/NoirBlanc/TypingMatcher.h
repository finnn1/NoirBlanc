// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TypingMatcher.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API UTypingMatcher : public UObject
{
	GENERATED_BODY()

public:
	UTypingMatcher();
	
	UPROPERTY(EditAnywhere)
	FString TargetString;
	
private:
	int CurrentPressedKeyIndex = 0;
	
public:
	bool CheckKeyIsRight(const FText PressedKey);
	void IncreaseKeyIndex();
	bool IsComplete() const;
	void Reset();
	
};
