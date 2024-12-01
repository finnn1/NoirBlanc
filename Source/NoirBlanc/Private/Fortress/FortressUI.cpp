// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/FortressUI.h"
#include "Components/ProgressBar.h"
#include "Fortress/Cannon.h"
#include "Kismet/GameplayStatics.h"


void UFortressUI::NativeConstruct()
{
	Super::NativeConstruct();
	
    Player1 = Cast<ACannon>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	Player1Percentage = 1.0f;
	Player2Percentage = 1.0f;
}

void UFortressUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFortressUI::ChangeHPBar(ACannon* Cannon)
{
	float percentage = (Cannon->Health / Cannon->MaxHealth) * 100;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Percent Changed"));
	// Player2pg->SetPercent(percentage);	// not working
}

void UFortressUI::ApplyDamageHPBar(ACannon* Cannon)
{
	float percentage = Cannon->Health / Cannon->MaxHealth;
	// Player2pg->SetPercent(percentage);	// not working
	Player2Percentage = percentage;
	UE_LOG(LogTemp, Warning, TEXT("Player2Percentage: %f"), Player2Percentage);
}

void UFortressUI::TakeDamageHPBar(ACannon* Cannon)
{
	float percentage = Cannon->Health / Cannon->MaxHealth;
	Player1Percentage = percentage;
	UE_LOG(LogTemp, Warning, TEXT("Player1Percentage: %f"), Player1Percentage);
}

