// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PawnCardSpawner.generated.h"

class APawnCard;

USTRUCT(BlueprintType)
struct FMyStruct
{
	//: public FTableRowBase
	GENERATED_BODY()
	
	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	FVector DestLocation;
	
	UPROPERTY()
	float ElapsedTime = 0;

	UPROPERTY()
	float Duration = 10;
};

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

	UPROPERTY()
	int32 CardMarginX = 100;

	UPROPERTY()
	int32 CardMarginZ = 120;

	UPROPERTY()
	APawnCard* SpawnedCard;

	//배열 초기화
	void InitCardsArray();
	
	//카드 배치
	void DistributeCards(TArray<TSubclassOf<APawnCard>> ShuffledCards);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<APawnCard*, FMyStruct> MapUseStr;

	float Duration = 1.f;
	float InRate = 0.03;

	void SetMoveTimer(APawnCard* Card);
	void UpdateLocationLerp(APawnCard* Card);

	void ShuffleCard();

private:
	UPROPERTY()
	TArray<APawnCard*> CardsOnLevel;
};
