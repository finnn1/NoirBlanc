// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "PawnCard.h"
#include "TurnCardActorComp.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFinishCardTurn, APawnCard*, APawnCard*);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOIRBLANC_API UTurnCardActorComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTurnCardActorComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	FOnFinishCardTurn OnFinishCardTurn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* MovingCurve;

	void TurnSelectCard(APawnCard* TargetCard);

	void RollbackCard(APawnCard* FirstSel, APawnCard* SecondSel);

	UPROPERTY()
	TArray<APawnCard*> CardsReturnToBack;

private:
	TWeakObjectPtr<APawnCard> FirstSelectedCard;
	TWeakObjectPtr<APawnCard> SecondSelectedCard;

	FRotator FrontRotation;
	FRotator BackRotation;
	
	FOnTimelineFloat StartTurnFloat;
	FOnTimelineEvent EndTurnEvent;

	UFUNCTION()
	void StartTurnLerp(float value);

	UFUNCTION()
	void EndTurnLerp();
		
};
