// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Knight.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MainUI.h"
#include "Blueprint/UserWidget.h"
#include "NoirBlanc/Knight/GameStateBase_Knight.h"
#include "FinishUI.h"
#include "NoirBlancGameInstance.h"
#include "Net/UnrealNetwork.h"

//DEFINE_ENUM_TO_STRING(EPieceColor);

// Sets default values
APlayer_Knight::APlayer_Knight()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}		

void APlayer_Knight::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->ConnectedPlayers += 1;
	if(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->ConnectedPlayers == 2)
	{
		/* Create Client UI */
		ClientRPC_CreateUI();
		
		/* Create Server UI */
		CountDownUI = Cast<UCountDownUI>(CreateWidget(GetWorld(), CountDownFactory));
		CountDownUI->AddToViewport();
		CountDownUI->UpdateCountDown(FText::AsNumber(CountDownLeft));

		/* Find Other Player */
		Road = Cast<ARoad>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoad::StaticClass()));
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Knight::StaticClass(), actors);
		for (AActor* Actor : actors)
		{
			Road->AllPlayers.Push(Cast<APlayer_Knight>(Actor));
			APlayer_Knight* tmpPlayer = Cast<APlayer_Knight>(Actor);
			if(OtherPlayer == nullptr && (IsLocallyControlled() && tmpPlayer->IsLocallyControlled() == false) ||
				(!IsLocallyControlled() && tmpPlayer->IsLocallyControlled() == true))
			{
				OtherPlayer = tmpPlayer;
			}
		}

		/* Start CountDown */
		GetWorldTimerManager().SetTimer(Handle, this, &APlayer_Knight::CountDown, 1, true);
	}
}

// Called every frame
void APlayer_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* CountDown */
	if(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Started == false)
	{
		return;
	}

	/* Finished */
	if(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Finished)
	{
		if(Main != nullptr)
		{
			Main->PlayerDisappear();
		}
		
		if(IsLocallyControlled() && FinishUI == nullptr)
		{
			FinishUI = Cast<UFinishUI>(CreateWidget(GetWorld(), FinishUIFactory));
			FinishUI->AddToViewport();
			FinishUI->UpdateWinnerText(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Winner);

			// 타이머로 5초 뒤에 다시 체스로 돌아가면 될듯
		}
		
		return;
	}
	
	/* Movement */
	{
		if (GetActorLocation().Z < -1000)
		{
			SetActorLocation(FVector(0, 0, 0));
			if (IsLocallyControlled())
			{
				Controller->SetControlRotation(FRotator::ZeroRotator);
			}
		}
		else
		{
			AddMovementInput(GetActorForwardVector());
		}
	}

	/* UI */
	if (HasAuthority() && OtherPlayer != nullptr)
	{
		if(TotalDistance > OtherPlayer->TotalDistance)
		{
			Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Winner = FText::FromString(TEXT("블랑"));
		}
		else
		{
			Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Winner = FText::FromString(TEXT("느와르"));
		}
		
		MulticastRPC_UpdateDistanceUI(TotalDistance, OtherPlayer->TotalDistance);
	}

	
	/* Save Result to GameInstance */
	if(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Winner.EqualTo(FText::FromString(TEXT("블랑"))))
	{
		Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance())->WinnerColor = EPieceColor::White;
	}
	else
	{
		Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance())->WinnerColor = EPieceColor::Black;
	}
	
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Server : %hhd"), Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance())->WinnerColor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client : %hhd"), Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance())->WinnerColor);
		//UE_LOG(LogTemp, Warning, TEXT("Client : %s"), *EnumToString(Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance())->WinnerColor));
	}
}


// ----------------------------------------------------------------------------------------
//
// FUNCTION
//

// -----------------------------------------
//
// COUNTDOWN
//
void APlayer_Knight::ClientRPC_CreateUI_Implementation()
{
	if(IsLocallyControlled())
	{
		CountDownUI = Cast<UCountDownUI>(CreateWidget(GetWorld(), CountDownFactory));
		CountDownUI->AddToViewport();
		CountDownUI->UpdateCountDown(FText::AsNumber(CountDownLeft));
	}
}

void APlayer_Knight::CountDown()
{
	CountDownLeft -= 1;
	if(CountDownLeft < 0)
	{
		Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Started = true;
		
		CountDownUI->RemoveFromParent();
		
		Main = Cast<UMainUI>(CreateWidget(GetWorld(), MainUI));
		Main->AddToViewport();
		
		GetWorldTimerManager().ClearTimer(Handle);
		GetWorldTimerManager().SetTimer(Handle, this, &APlayer_Knight::StartTimer, 1, true);
	}
	else
	{
		if(CountDownLeft == 0)
		{
			CountDownUI->UpdateCountDown(FText::FromString(TEXT("시작!")));
		}
		else
		{
			CountDownUI->UpdateCountDown(FText::AsNumber(CountDownLeft));
		}
	}
}

void APlayer_Knight::OnRep_CountDownLeft()
{
	if(IsLocallyControlled())
	{
		if(CountDownLeft < 0)
		{
			CountDownUI->RemoveFromParent();

			Main = Cast<UMainUI>(CreateWidget(GetWorld(), MainUI));
			Main->AddToViewport();
		}
		else
		{
			if(CountDownLeft == 0)
			{
				CountDownUI->UpdateCountDown(FText::FromString(TEXT("시작!")));
			}
			else
			{
				CountDownUI->UpdateCountDown(FText::AsNumber(CountDownLeft));
			}
		}
	}
}

// -----------------------------------------
//
// TIMER
//
void APlayer_Knight::StartTimer()
{
	TimeLeft -= 1;
	if(TimeLeft == 0 || Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Finished)
	{
		Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Finished = true;
		GetWorldTimerManager().ClearTimer(Handle);
	}
	
	MulticastRPC_UpdateTimerUI();
}

void APlayer_Knight::MulticastRPC_UpdateTimerUI_Implementation()
{
	if(Main != nullptr)
	{
		Main->UpdateTimerText(TimeLeft);
	}
}

// -----------------------------------------
//
// DISTANCE
//

void APlayer_Knight::MulticastRPC_UpdateDistanceUI_Implementation(float serverDistance, float clientDistance)
{
	if(Main != nullptr)
	{
		Main->UpdateServerDistance(clientDistance);
		Main->UpdateClientDistance(serverDistance);
	}
}

void APlayer_Knight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayer_Knight, CountDownLeft);
	DOREPLIFETIME(APlayer_Knight, TimeLeft);
}

// Called to bind functionality to input
void APlayer_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
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

void APlayer_Knight::Move(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void APlayer_Knight::Jump()
{
	Super::Jump();
}
