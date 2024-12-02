// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateBase_Knight.h"

#include "FinishUI.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Knight.h"
#include "Net/UnrealNetwork.h"

AGameStateBase_Knight::AGameStateBase_Knight()
{
	bReplicates = true;
}

void AGameStateBase_Knight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameStateBase_Knight, CountDownLeft);
	DOREPLIFETIME(AGameStateBase_Knight, Started);
	DOREPLIFETIME(AGameStateBase_Knight, Finished);
	DOREPLIFETIME(AGameStateBase_Knight, GameTimeLeft);
	DOREPLIFETIME(AGameStateBase_Knight, Winner);
}

void AGameStateBase_Knight::StartCountDown()
{
	if(HasAuthority())
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AGameStateBase_Knight::MulticastRPC_CreateCountDownUI, 1, false);
	
		//MulticastRPC_CreateCountDownUI();
	}
}

void AGameStateBase_Knight::MulticastRPC_CreateCountDownUI_Implementation()
{
	CountDownUI = Cast<UCountDownUI>(CreateWidget(GetWorld(), CountDownFactory));
	CountDownUI->AddToViewport();
	CountDownUI->Txt_Count->SetText(FText::AsNumber(CountDownLeft));

	if(HasAuthority())
	{
		GetWorldTimerManager().SetTimer(Handle, this, &AGameStateBase_Knight::CountDown, 1, true);
	}
}


void AGameStateBase_Knight::CountDown()
{
	CountDownLeft -= 1;
	if(CountDownLeft == 0)
	{
		CountDownUI->Txt_Count->SetText(FText::FromString(TEXT("시작!")));
	}
	else if(CountDownLeft < 0)
	{
		CountDownUI->RemoveFromParent();
		GetWorldTimerManager().ClearTimer(Handle);
		//Started = true;

		GetWorldTimerManager().SetTimer(Handle, this, &AGameStateBase_Knight::StartTimer, 1, true);
	}
	else
	{
		CountDownUI->Txt_Count->SetText(FText::AsNumber(CountDownLeft));
	}
}

void AGameStateBase_Knight::StartTimer()
{
	GameTimeLeft -= 1;
	APlayer_Knight* serverPlayer = Cast<APlayer_Knight>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(serverPlayer != nullptr && serverPlayer->OtherPlayer != nullptr && serverPlayer->TotalDistance > serverPlayer->OtherPlayer->TotalDistance)
	{
		Winner = FText::FromString(TEXT("느와르"));
	}
	else
	{
		Winner = FText::FromString(TEXT("블랑"));
	}
	
	if(GameTimeLeft == 0)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}
}

void AGameStateBase_Knight::MulticastRPC_CreateFinishUI_Implementation()
{
	FinishUI = Cast<UFinishUI>(CreateWidget(GetWorld(), FinishUIFactory));
	FinishUI->AddToViewport();
	FinishUI->UpdateWinnerText(Winner);
}

void AGameStateBase_Knight::OnRep_CountDownLeft()
{
	/*
	if(CountDownLeft == 0)
	{
		CountDownUI->Txt_Count->SetText(FText::FromString(TEXT("시작!")));
	}
	else if(CountDownLeft < 0)
	{
		CountDownUI->RemoveFromParent();
	}
	else
	{
		CountDownUI->Txt_Count->SetText(FText::AsNumber(CountDownLeft));
	}
	*/
}

