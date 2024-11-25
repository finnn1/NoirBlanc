// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPlayerPawn.h"

#include "ChessBoard.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AChessPlayerPawn::AChessPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AChessPlayerPawn::Click(const FInputActionValue& Value)
{
	ClickDelegate.Broadcast();
}

// Called when the game starts or when spawned
void AChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();


	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// Enhanced Input Subsystem을 가져오기
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (InputSubsystem)
		{
			// 입력 매핑 컨텍스트 추가
			InputSubsystem->AddMappingContext(InputMappingContext, 0);  
		}
	}
	AActor* Board = UGameplayStatics::GetActorOfClass(GetWorld(), AChessBoard::StaticClass());
	AChessBoard* ChessBoard = Cast<AChessBoard>(Board);
	if(ChessBoard)
	{
		ClickDelegate.AddUObject(ChessBoard, &AChessBoard::ClickFloor);
	}
}

// Called every frame
void AChessPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChessPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AChessPlayerPawn::Click);
	}
}

