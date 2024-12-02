// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardFloor.generated.h"

class UBoxComponent;
class AChessPiece;
UCLASS()
class NOIRBLANC_API ABoardFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardFloor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	AChessPiece* GetPieceOnFloor();
	void SetPieceOnFloor(AChessPiece* Piece);
	int32 GetRow();
	int32 GetCol();
	void SetRow(int32 Row);
	void SetCol(int32 Col);
	
	void ToggleGreen();
	void ToggleRed();
	void ToggleBlue();
//////////Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompCollision")
	UBoxComponent* CompBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompGreen")
	UStaticMeshComponent* CompGreen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompBlue")
	UStaticMeshComponent* CompBlue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompRed")
	UStaticMeshComponent* CompRed;
	
private:
	UPROPERTY(Replicated,VisibleAnywhere, Category = "FloorInfo")
	AChessPiece* PieceOnFloor = nullptr;

	UPROPERTY(Replicated,VisibleAnywhere, Category = "FloorInfo")
	int32 FloorRow;

	UPROPERTY(Replicated,VisibleAnywhere, Category = "FloorInfo")
	int32 FloorCol;
};
