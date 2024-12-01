// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightPlayerState.h"
#include "Net/UnrealNetwork.h"

void AKnightPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AKnightPlayerState, TotalDistance);
}
