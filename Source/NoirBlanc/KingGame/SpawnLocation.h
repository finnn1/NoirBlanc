// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocation.generated.h"

UCLASS()
class NOIRBLANC_API ASpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	ASpawnLocation();

	bool bIsSelected = false;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* FrontArrow;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void Select();
	void Deselect();

public:
	class UMaterialInstanceDynamic* DynamicMaterial;
	
	void ColorToRed();
	void ColorToWhite();
};