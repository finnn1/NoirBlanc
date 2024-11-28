// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PawnCardSpawner.generated.h"

class APawnCard;
UCLASS()
class NOIRBLANC_API APawnCardSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APawnCardSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<APawnCard>> Cards;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<APawnCard*, FVector> CardLocationMap;

	UPROPERTY()
	int32 CardMarginX = 100;

	UPROPERTY()
	int32 CardMarginZ = 120;

	UPROPERTY()
	APawnCard* SpawnedCard;

	//카드 셔플
	void ShuffleCards();
	
	//카드 배치
	void DistributeCards(TArray<TSubclassOf<APawnCard>> ShuffledCards);
	
public:
	UPROPERTY()
	UTimelineComponent* Timeline;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* MovingLinearCurve;

	FOnTimelineFloat StartMoveFloat;
	FOnTimelineEvent EndMoveEvent;
	

	UFUNCTION()
	void StartMoveCard(float value);

	UFUNCTION()
	void EndMoveCard();

private:
	UPROPERTY()
	TArray<APawnCard*> CardsOnLevel;
};
