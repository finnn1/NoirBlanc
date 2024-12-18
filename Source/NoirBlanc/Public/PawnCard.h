// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PawnCard.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishSetMat, APawnCard* PawnCard);
UENUM(BlueprintType)
enum class CardState : uint8
{
	Front,
	Back
};
class UPawnCardDataAsset;
class AFieldSystemActor;

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
	FOnFinishSetMat OnFinishSetMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMeshComp;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UGeometryCollectionComponent* GeometryCollectionComp;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default Data")
	UPawnCardDataAsset* PawnCardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default Data")
	CardState FrontBackState;

	// Actor 흔들기 - 타임라인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* ShakeTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* ShakingCurve;
	
	FOnTimelineFloat StartShakeFloat;
	FOnTimelineEvent EndShakeEvent;

	// Shake 시작
	UFUNCTION()
	void StartShakeFunc(float value);

	// Shake 끝난 후 이벤트
	UFUNCTION()
	void EndShakeFunc();

	// 셔플 후 게임 시작할 때의 원점 위치
	UPROPERTY()
	FVector OriginLocation;
	
private:
	UPROPERTY()
	APlayerState* OwnerPlayerState;
	
	UPROPERTY()
	bool bIsSelectable;

	void StartLerpMaterial();
	
	FTimerHandle LerpTimer;

	float CurrentLerpTime;
	float LerpCycle = 0.04;
	

public:
	//카드 앞뒷면 상태 변화
	void ChangeFrontBackState();

	//초기 상태로
	void InitCard();

	//매칭에 성공
	int32 MatchingSuccess();
	
	//매칭 취소 함수
	void CancelMatching();

	//Chaos Distruction
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<AFieldSystemActor> MasterFieldActor;*/
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartPhyicsSimul();

	//Destroy Delegate
	UFUNCTION(BlueprintCallable)
	void BroadcastEndDestruction();

	//Material Dissolve
	void DissolvePawnCardMat();
	
	void StartShakeTimeline();
};
