// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Knight.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "FinishUI.h"
#include "MainUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
// Sets default values
APlayer_Knight::APlayer_Knight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void APlayer_Knight::BeginPlay()
{
	Super::BeginPlay();
	Main = Cast<UMainUI>(CreateWidget(GetWorld(), MainUI));
	Main->AddToViewport();
}

// Called every frame
void APlayer_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Ended)
	{
		if(HasAuthority())
		{
			if(Timer > 0)
			{
				Timer -= DeltaTime;
			}
			else
			{
				Ended = true;
			}
		}
	
		Main->UpdateTimerText(Timer);
		
		if(GetActorLocation().Z < -500)
		{
			SetActorLocation(FVector(0, 0, 190));
			if(IsLocallyControlled())
			{
				Controller->SetControlRotation(FRotator::ZeroRotator);
			}

		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Yaw : %f"), Controller->GetControlRotation().Yaw);
			AddMovementInput(GetActorForwardVector());
		}
	}
	else
	{
		if(!Finished)
		{
			Finish = Cast<UFinishUI>(CreateWidget(GetWorld(), FinishUI));
			Finish->AddToViewport();
			Finished = true;
		}

	}
}

// Called to bind functionality to input
void APlayer_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayer_Knight::Move);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APlayer_Knight::Jump);
	}

}

void APlayer_Knight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayer_Knight, Timer);
}

void APlayer_Knight::Move(const FInputActionValue& Value)
{

	AddControllerYawInput(Value.Get<float>());
	
}

void APlayer_Knight::Jump()
{
	Super::Jump();
}	

