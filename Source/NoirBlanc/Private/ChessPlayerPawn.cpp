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

void AChessPlayerPawn::LeftClick(const FInputActionValue& Value)
{
	if(!IsLocallyControlled()) return;
	ClickDelegate.Broadcast();
}

void AChessPlayerPawn::RightClick(const FInputActionValue& Value)
{
	if(!IsLocallyControlled()) return;
	bIsRotMode = !bIsRotMode;
}

void AChessPlayerPawn::Look(const FInputActionValue& Value)
{
	if(!IsLocallyControlled()) return;
	if(!bIsRotMode) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
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
		ChessBoard->SetOwner(this);
		if(IsLocallyControlled())
		{
			if(HasAuthority())
			{

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, ChessBoard]()
				{
					ChessBoard->InitFloor();
					ChessBoard->InitBoard();	
				}, 3, false);				
				
			}
		}
	}
	if(HasAuthority())
	{
		if(IsLocallyControlled())
		{
			SetActorLocation(FVector(150.f,3500.f,4000.f));
			GetController()->SetControlRotation(FRotator(310.f,-90.f,0.f));
			// SetActorRotation();
		}
	}
	else
	{
		if(IsLocallyControlled())
		{
			SetActorLocation(FVector(150.f,-3000.f,4000.f));
			GetController()->SetControlRotation(FRotator(-50.f,90.f,0.f));
		}
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
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AChessPlayerPawn::LeftClick);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AChessPlayerPawn::RightClick);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AChessPlayerPawn::Look);
	}
}

