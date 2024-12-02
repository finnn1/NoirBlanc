// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlancPlayerState.h"
#include "Net/UnrealNetwork.h"

void ANoirBlancPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate 할 변수 등록
	DOREPLIFETIME(ANoirBlancPlayerState, bIsAttaker);
}
