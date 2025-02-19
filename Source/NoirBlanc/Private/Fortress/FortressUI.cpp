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
#include "NoirBlanc/Knight/TurnUI.h"
#include "NoirBlanc/Knight/FinishUI.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PieceTypes.h"


void UFortressUI::NativeConstruct()
{
	Super::NativeConstruct();
	
    playerCannon = Cast<ACannon>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	Player1Percentage = 1.0f;
	Player2Percentage = 1.0f;

	playerPieceColor = EPieceColor::White;
	turnUI->ShowTurn(playerPieceColor);

	if (playerCannon->HasAuthority())
	{
		Player1Info->SetText(FText::FromString(TEXT("블랑")));
		Player2Info->SetText(FText::FromString(TEXT("느와르")));
		Player1pg->SetFillColorAndOpacity(FLinearColor::White);
		Player2pg->SetFillColorAndOpacity(FLinearColor::Black);
	}

	if (ImageHorizontalBox != nullptr)
	{
		for (UWidget* Child: ImageHorizontalBox->GetAllChildren())
			if (UImage* Image = Cast<UImage>(Child)) ArrowImages.Add(Image);
	}
	
	for (int32 i = 0; i < ArrowImages.Num(); i++)
		ArrowImages[i]->SetVisibility(ESlateVisibility::Hidden);
	
}

	// APlayerController* pc = GetWorld()->GetFirstPlayerController();
	// if (pc)
	// 	playerCannon->DisableInput(pc);
	//playerCannon->GetMovementComponent()->Deactivate();

	// CountdownTime = 0;
	// GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &UFortressUI::UpdateCountdown, 1.0f, true);

void UFortressUI::UpdateCountdown()
{	
	//text_Countdown->SetText(FText::FromString(FString::FromInt(3-CountdownTime)));
	++CountdownTime;
	if (CountdownTime > 3)
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimer);
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

		//playerCannon->GetMovementComponent()->Activate();
		// APlayerController* pc = GetWorld()->GetFirstPlayerController();
		// if (pc)
		// 	playerCannon->EnableInput(pc);

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
	}
	else
	{
		Player2Percentage = percentage;
	}
}

void UFortressUI::GameOver(int32 index)
{
	UE_LOG(LogTemp, Warning, TEXT("Index %d"), index);
	FText winner = index == 0 ? FText::FromString(TEXT("블랑")) : FText::FromString(TEXT("느와르"));
	UE_LOG(LogTemp, Warning, TEXT("Winner %s"), *winner.ToString());
	FinishUI->UpdateWinnerText(winner);
	WidgetSwitcher->SetActiveWidgetIndex(3);
	
	// save result to game instance
	UNoirBlancGameInstance* gi = Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance());
	gi->WinnerColor = index == 0 ? EPieceColor::White : EPieceColor::Black;
	Cast<ATravelPlayerController>(GetWorld()->GetFirstPlayerController())->ServerRPC_LevelTravelToChess();
}

// turn UI
// void UFortressUI::SetTurnWidgetVisible()
// {
// 	// set widget visible to know whose turn is it
// 	if (playerCannon) // server doesn't have playerCannon
// 	{
// 		turnUI->ShowTurn(playerPieceColor);
// 	}
// }

void UFortressUI::SetWindBar(int32 Strength)
{
	for (int32 i = 0; i < ArrowImages.Num(); i++)
		ArrowImages[i]->SetVisibility(ESlateVisibility::Hidden);
	
	if (playerCannon->bIsturn)
	{
		switch (Strength)
		{
			case 1:
				ArrowImages[3]->SetVisibility(ESlateVisibility::Visible);
				break;
			case 2:
				ArrowImages[3]->SetVisibility(ESlateVisibility::Visible);
				ArrowImages[4]->SetVisibility(ESlateVisibility::Visible);
				break;	
			case 3:
				ArrowImages[3]->SetVisibility(ESlateVisibility::Visible);
				ArrowImages[4]->SetVisibility(ESlateVisibility::Visible);
				ArrowImages[5]->SetVisibility(ESlateVisibility::Visible);
				break;
			case -1:
				ArrowImages[2]->SetVisibility(ESlateVisibility::Visible);
				break;
			case -2:
				ArrowImages[2]->SetVisibility(ESlateVisibility::Visible);
				ArrowImages[1]->SetVisibility(ESlateVisibility::Visible);
				break;	
			case -3:
				ArrowImages[2]->SetVisibility(ESlateVisibility::Visible);
				ArrowImages[1]->SetVisibility(ESlateVisibility::Visible);
				ArrowImages[0]->SetVisibility(ESlateVisibility::Visible);
				break;
			default:
				break;
		}
	}
	else
	{
		switch (Strength)
		{
		case -1:
			ArrowImages[3]->SetVisibility(ESlateVisibility::Visible);
			break;
		case -2:
			ArrowImages[3]->SetVisibility(ESlateVisibility::Visible);
			ArrowImages[4]->SetVisibility(ESlateVisibility::Visible);
			break;	
		case -3:
			ArrowImages[3]->SetVisibility(ESlateVisibility::Visible);
			ArrowImages[4]->SetVisibility(ESlateVisibility::Visible);
			ArrowImages[5]->SetVisibility(ESlateVisibility::Visible);
			break;
		case 1:
			ArrowImages[2]->SetVisibility(ESlateVisibility::Visible);
			break;
		case 2:
			ArrowImages[2]->SetVisibility(ESlateVisibility::Visible);
			ArrowImages[1]->SetVisibility(ESlateVisibility::Visible);
			break;	
		case 3:
			ArrowImages[2]->SetVisibility(ESlateVisibility::Visible);
			ArrowImages[1]->SetVisibility(ESlateVisibility::Visible);
			ArrowImages[0]->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			break;
		}
	}
}



