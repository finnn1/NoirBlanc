// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/FortressUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Fortress/Cannon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"




void UFortressUI::NativeConstruct()
{
	Super::NativeConstruct();
	
    Player1 = Cast<ACannon>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	Player1Percentage = 1.0f;
	Player2Percentage = 1.0f;

	CountdownTime = 0;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &UFortressUI::UpdateCountdown, 1.0f, true);
}

void UFortressUI::UpdateCountdown()
{	
	text_Countdown->SetText(FText::FromString(FString::FromInt(3-CountdownTime)));
	++CountdownTime;
	if (CountdownTime > 3)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimer);
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
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

void UFortressUI::ApplyDamageHPBar(ACannon* Cannon, ACannon*  player)
{
	float percentage = Cannon->Health / Cannon->MaxHealth;

	if (Cannon == player)
	{
		Player1Percentage = percentage;
		UE_LOG(LogTemp, Warning, TEXT("Player1Percentage: %f"), Player1Percentage);
	}
	else
	{
		// Player2pg->SetPercent(percentage);	// not working
		Player2Percentage = percentage;
		UE_LOG(LogTemp, Warning, TEXT("Player2Percentage: %f"), Player2Percentage);
	}
}

void UFortressUI::TakeDamageHPBar(ACannon* Cannon)
{
	float percentage = Cannon->Health / Cannon->MaxHealth;
	Player1Percentage = percentage;
	UE_LOG(LogTemp, Warning, TEXT("Player1Percentage: %f"), Player1Percentage);
}

void UFortressUI::GameOver(int32 index)
{
	FText winner = index == 0 ? FText::FromString(TEXT("Noir")) : FText::FromString(TEXT("Blanc"));
	text_Winner->SetText(winner);
	WidgetSwitcher->SetActiveWidgetIndex(2);
}