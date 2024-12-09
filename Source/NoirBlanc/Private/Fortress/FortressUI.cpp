// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/FortressUI.h"

#include "NoirBlancGameInstance.h"
#include "TravelPlayerController.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Fortress/Cannon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PawnMovementComponent.h"


void UFortressUI::NativeConstruct()
{
	Super::NativeConstruct();
	
    playerCannon = Cast<ACannon>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	Player1Percentage = 1.0f;
	Player2Percentage = 1.0f;

	horizontalBox_Turn->SetVisibility(ESlateVisibility::Hidden);

	// APlayerController* pc = GetWorld()->GetFirstPlayerController();
	// if (pc)
	// 	playerCannon->DisableInput(pc);
	//playerCannon->GetMovementComponent()->Deactivate();
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
		//playerCannon->GetMovementComponent()->Activate();
		// APlayerController* pc = GetWorld()->GetFirstPlayerController();
		// if (pc)
		// 	playerCannon->EnableInput(pc);
	}

	// MulticastRPC -> UI 업데이트 해라!!
	
}

void UFortressUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFortressUI::ApplyDamageHPBar(ACannon* damagedCannon, ACannon* player)
{
	float percentage = damagedCannon->Health / damagedCannon->MaxHealth;

	if (damagedCannon == player)
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

void UFortressUI::GameOver(int32 index)
{
	FText winner = index == 0 ? FText::FromString(TEXT("Blanc")) : FText::FromString(TEXT("Noir"));
	text_Winner->SetText(winner);
	WidgetSwitcher->SetActiveWidgetIndex(2);

	// save result to game instance
	UNoirBlancGameInstance* gi = Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance());
	gi->WinnerColor = index == 0 ? EPieceColor::White : EPieceColor::Black;
	Cast<ATravelPlayerController>(GetWorld()->GetFirstPlayerController())->ServerRPC_LevelTravelToChess();
}

// turn UI
void UFortressUI::SetTurnWidgetVisible()
{
	// set widget visible to know whose turn is it
	if (playerCannon) // server doesn't have playerCannon
	{
		//UE_LOG(LogTemp, Warning, TEXT("UI turnCannon %s"), *playerCannon->turnCannon.ToString());
		text_Turn->SetText(playerCannon->turnCannon);
	}
		
	horizontalBox_Turn->SetVisibility(ESlateVisibility::Visible);
	// delay for 2 sec
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UFortressUI::SetTurnWidgetHidden, 1.0f, false);
}

void UFortressUI::SetTurnWidgetHidden()
{
	horizontalBox_Turn->SetVisibility(ESlateVisibility::Hidden);
}
