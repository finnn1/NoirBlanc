// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

#include "FinishUI.h"
#include "Player_Knight.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
AMyGameStateBase::AMyGameStateBase()
{
	bReplicates = true;
}

void AMyGameStateBase::StartTimer()
{
	if (HasAuthority()) // 서버에서만 실행
	{
		GetWorld()->GetTimerManager().SetTimer(
			handle,
			this,
			&AMyGameStateBase::UpdateRemainingTime,
			1.0f, // 1초 간격
			true  // 루프 실행
		);
	}
}

void AMyGameStateBase::UpdateRemainingTime()
{
	remainingTime-=1;
	if(remainingTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(handle);
		
		
		if(HasAuthority())
		{
			APlayer_Knight* serverPlayer = Cast<APlayer_Knight>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if(serverPlayer->TotalDistance > serverPlayer->OtherPlayer->TotalDistance)
			{
				Winner = FText::FromString("Blanc");
			}
			else
			{
				Winner = FText::FromString("Noir");
			}
			
			MulticastRPC_CreateFinishUI();
		}
	}
}

void AMyGameStateBase::MulticastRPC_CreateFinishUI_Implementation()
{
	TimeOver = true;
	FinishUI = Cast<UFinishUI>(CreateWidget(GetWorld(), FinishUIFactory));
	FinishUI->AddToViewport();
	FinishUI->UpdateWinnerText(Winner);
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameStateBase, remainingTime);
	DOREPLIFETIME(AMyGameStateBase, Winner);
}
