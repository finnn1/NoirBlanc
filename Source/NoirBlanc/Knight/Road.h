// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.generated.h"

UCLASS()
class NOIRBLANC_API ARoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* SplineComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float X_Length = 0;
	
	TArray<class APlayer_Knight*> AllPlayers;
};	
