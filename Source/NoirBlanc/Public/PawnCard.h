// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PawnCard.generated.h"

class UPawnCardDataAsset;

UENUM(BlueprintType)
enum class CardState : uint8
{
	Front,
	Back
};

UCLASS()
class NOIRBLANC_API APawnCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APawnCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default Data")
	UPawnCardDataAsset* PawnCardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default Data")
	CardState FrontBackState;
	
	UPROPERTY()
	bool bIsMatched;

	UPROPERTY()
	bool bIsSelectable;

private:
	UPROPERTY()
	APlayerState* OwnerPlayerState;

public:
	//카드 뒤집는 함수
	void TurnToFront();
	void TurnToBack();

	//매칭 성공했을 때 함수
	void CompletedMatching();

	//매칭 취소 함수
	void CancelMatching();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* MovingCurve;
	
	FOnTimelineFloat StartTurnFloat;

	UFUNCTION()
	void StartTurnLerp(float value);
};
