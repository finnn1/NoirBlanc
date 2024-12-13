// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPiece.h"
#include "Components/BoxComponent.h"

#include "HLSLTree/HLSLTreeTypes.h"
#include "Misc/MapErrors.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CompMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompMesh"));
	CompMesh->SetSimulatePhysics(true);
	RootComponent = CompMesh;

	CompBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CompBox"));
	CompBox->SetupAttachment(RootComponent);
	CompBox->SetWorldScale3D(FVector(10.f, 10.f, 15.f));
}
// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &AChessPiece::SetPieceMesh, 0.2f, false);
}

void AChessPiece::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChessPiece, PieceType);
	DOREPLIFETIME(AChessPiece, PieceColor);
	DOREPLIFETIME(AChessPiece, FloorBeneathPiece);
	DOREPLIFETIME(AChessPiece, MoveCount);
	DOREPLIFETIME(AChessPiece, EncounterCount);
}

EPieceType AChessPiece::GetPieceType()
{
	return PieceType;
}

EPieceColor AChessPiece::GetPieceColor()
{
	return PieceColor;
}

ABoardFloor* AChessPiece::GetFloorBeneathPiece()
{
	return FloorBeneathPiece;
}

void AChessPiece::SetPieceType(EPieceType Type)
{
	PieceType = Type;
}

void AChessPiece::SetPieceColor(EPieceColor Color)
{
	PieceColor = Color;
}

void AChessPiece::SetFloorBeneathPiece(ABoardFloor* Floor)
{
	FloorBeneathPiece = Floor;
}

void AChessPiece::IncreaseMoveCount()
{
	MoveCount++;
}

void AChessPiece::IncreaseEncounterCount()
{
	EncounterCount++;
}

void AChessPiece::SetMoveCount(int32 Count)
{
	MoveCount = Count;
}

void AChessPiece::SetEncounterCount(int32 Count)
{
	EncounterCount = Count;
}

int32 AChessPiece::GetMoveCount()
{
	return MoveCount;
}

int32 AChessPiece::GetEncounterCount()
{
	return EncounterCount;
}

void AChessPiece::DissolveMaterial()
{
	UMaterialInterface* Material = CompMesh->GetMaterial(0); // 0은 첫 번째 Material Slot
	if (Material)
	{
		// Material Instance Dynamic 생성
		UMaterialInstanceDynamic* Dynamic = UMaterialInstanceDynamic::Create(Material, this);
        
		// Material Instance Dynamic을 StaticMeshComponent에 설정
		CompMesh->SetMaterial(0, Dynamic);

		// Scalar Parameter 값 설정 (예: "MyScalarParam" 이름의 Scalar 파라미터)
		if (Dynamic)
		{
			
			GetWorld()->GetTimerManager().SetTimer(DissolveTimer, [this,Dynamic]()
				{
					float DeltaTime = GetWorld()->DeltaTimeSeconds;
					float StartValue = -0.4f;
					float EndValue = 0.7f;
					float InterpSpeed = 3.f;
					float Value = FMath::Lerp(StartValue, EndValue, DissolveCounter *InterpSpeed);
					DissolveCounter += DeltaTime;
					Dynamic->SetScalarParameterValue(TEXT("Dissolve"), Value);
					if(DissolveCounter >= 1.f)
					{
						StopTimer();
					}
				},
			0.05, true);
		}
	}
}

void AChessPiece::StopTimer()
{
	GetWorldTimerManager().ClearTimer(DissolveTimer);
}

void AChessPiece::SetPieceMesh()
{
	switch(PieceType)
	{
	case EPieceType::Pawn:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[0]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[1]);
		}
		break;
	case EPieceType::Knight:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[2]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[3]);
		}
		break;
	case EPieceType::Bishop:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[4]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[5]);
		}
		break;
	case EPieceType::Rook:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[6]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[7]);
		}
		break;
	case EPieceType::Queen:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[8]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[9]);
		}
		break;
	case EPieceType::King:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[10]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[11]);
		}
		break;
	}
}
