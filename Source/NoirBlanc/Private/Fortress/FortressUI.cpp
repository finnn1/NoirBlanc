// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/FortressUI.h"
#include "Components/ProgressBar.h"
#include "Fortress/Cannon.h"
#include "Kismet/GameplayStatics.h"


void UFortressUI::NativeConstruct()
{
	Super::NativeConstruct();
	
    Player1 = Cast<ACannon>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));

}

void UFortressUI::ChangeHPBar(ACannon* Cannon)
{
	float percentage = (Cannon->Health / Cannon->MaxHealth) * 100;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Percent Changed"));
	Player2pg->SetPercent(percentage);
}

