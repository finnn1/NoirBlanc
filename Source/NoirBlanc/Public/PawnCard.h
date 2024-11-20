// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY()
	APlayerState* OwnerPlayerState;
	
	UPROPERTY()
	bool bIsSelectable;

public:
	//카드 앞뒷면 상태 변화
	void ChangeFrontBackState();

	//초기 상태로
	void InitCard();

	//선택 됐을 때
	void Selected();

	//선택 가능한지 체크
	bool IsCardSelectable();

	//매칭에 성공했을 때 Owner 등록
	void SuccessMatching(APlayerController* OwnerPlayer);

	//매칭에 성공했는지
	bool GetOwnerPlayer();

	//매칭 취소 함수
	void CancelMatching();
};
