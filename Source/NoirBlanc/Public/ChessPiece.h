// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceTypes.h"

#include "ChessPiece.generated.h"

UCLASS()
class NOIRBLANC_API AChessPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	EPieceType GetPieceType();
	EPieceColor GetPieceColor();
	void SetPieceType(EPieceType Type);
	void SetPieceColor(EPieceColor Color);

protected:
	void SetPieceMesh();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompMesh")
	class UStaticMeshComponent* CompMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PieceMesh")
	TArray<UStaticMesh*> Meshes;
private:
	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	EPieceType PieceType;

	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	EPieceColor PieceColor;
};
