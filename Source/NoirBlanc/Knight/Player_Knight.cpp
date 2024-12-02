// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Knight.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MainUI.h"
#include "Blueprint/UserWidget.h"
#include "NoirBlanc/Knight/GameStateBase_Knight.h"
#include "FinishUI.h"
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
	
	/* Count Connected Players */
	ConnectedPlayers += 1;
	if(ConnectedPlayers == 2)
	{
		ServerRPC_StartGame();
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

	/* Time Over */
	if(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Finished)
	{
		if(IsLocallyControlled() && FinishUI == nullptr)
		{
			FinishUI = Cast<UFinishUI>(CreateWidget(GetWorld(), FinishUIFactory));
			FinishUI->AddToViewport();
			FinishUI->UpdateWinnerText(Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Winner);
		}
		
		return;
	}
	
	/* Movement */
	{
		if (GetActorLocation().Z < -500)
		{
			SetActorLocation(FVector(0, 0, 190));
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
		MulticastRPC_UpdateDistanceUI(TotalDistance, OtherPlayer->TotalDistance);
	}
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

void APlayer_Knight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayer_Knight, ConnectedPlayers);
	DOREPLIFETIME(APlayer_Knight, CountDownLeft);
	DOREPLIFETIME(APlayer_Knight, TimeLeft);
}

void APlayer_Knight::ServerRPC_StartGame_Implementation()
{

	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &APlayer_Knight::FindOtherPlayer, 3, false);
	MulticastRPC_CreateCountDown();
}

void APlayer_Knight::MulticastRPC_CreateCountDown_Implementation()
{
	Main = Cast<UMainUI>(CreateWidget(GetWorld(), MainUI));
	Main->AddToViewport();
	
	CountDownUI = Cast<UCountDownUI>(CreateWidget(GetWorld(), CountDownFactory));
	CountDownUI->AddToViewport();
	CountDownUI->Txt_Count->SetText(FText::AsNumber(CountDownLeft));
	
	if(HasAuthority())
	{
		GetWorldTimerManager().SetTimer(Handle, this, &APlayer_Knight::CountDown, 1, true);
	}
}

void APlayer_Knight::CountDown()
{
	CountDownLeft -= 1;
	if(CountDownLeft < 0)
	{
		Cast<AGameStateBase_Knight>(GetWorld()->GetGameState())->Started = true;
		
		CountDownUI->RemoveFromParent();
		GetWorldTimerManager().ClearTimer(Handle);
		GetWorldTimerManager().SetTimer(Handle, this, &APlayer_Knight::StartTimer, 1, true);
	}
	else
	{
		if(CountDownLeft == 0)
		{
			CountDownUI->Txt_Count->SetText(FText::FromString(TEXT("시작!")));
		}
		else
		{
			CountDownUI->Txt_Count->SetText(FText::AsNumber(CountDownLeft));
		}
	}
}

void APlayer_Knight::StartTimer()
{
	TimeLeft -= 1;
	if(TimeLeft == 0)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
	
	MulticastRPC_UpdateTimerUI();
}

void APlayer_Knight::MulticastRPC_UpdateTimerUI_Implementation()
{
	Main->UpdateTimerText(TimeLeft);
}

void APlayer_Knight::OnRep_CountDownLeft()
{
	if(CountDownLeft < 0)
	{
		CountDownUI->RemoveFromParent();
	}
	else
	{
		if(CountDownLeft == 0)
		{
			CountDownUI->Txt_Count->SetText(FText::FromString(TEXT("시작!")));
		}
		else
		{
			CountDownUI->Txt_Count->SetText(FText::AsNumber(CountDownLeft));
		}
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

void APlayer_Knight::FindOtherPlayer()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Knight::StaticClass(), actors);
	for (AActor* Actor : actors)
	{
		APlayer_Knight* tmpPlayer = Cast<APlayer_Knight>(Actor);
		
		if((IsLocallyControlled() && tmpPlayer->IsLocallyControlled() == false) ||
			(!IsLocallyControlled() && tmpPlayer->IsLocallyControlled() == true))
		{
			OtherPlayer = tmpPlayer;
			break;
		}
	}
}

void APlayer_Knight::MulticastRPC_UpdateDistanceUI_Implementation(float serverDistance, float clientDistance)
{
	Main->UpdateMyDistance(serverDistance);
	Main->UpdateEnemyDistance(clientDistance);
}
