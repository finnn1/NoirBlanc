// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingCatcherUI.h"

#include "KingCatcherFireButtonUI.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UKingCatcherUI::SetButtonColor(int32 ButtonIndex, bool IsSelected)
{
	for (UKingCatcherFireButtonUI* SpawnLocationButton : AllSpawnLocationButtons)
	{
		if (SpawnLocationButton->Index == ButtonIndex)
		{
			SpawnLocationButton->SetButtonSelected(IsSelected);
		}
	}
}

void UKingCatcherUI::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> Boxs = Panel_Buttons->GetAllChildren();

	for (UWidget* Box : Boxs)
	{
		UHorizontalBox* _HorizontalBox = Cast<UHorizontalBox>(Box);
		if (_HorizontalBox)
		{
			for (UWidget* ButtonWidet : _HorizontalBox->GetAllChildren())
			{
				UKingCatcherFireButtonUI* _SpawnLocationButton = Cast<UKingCatcherFireButtonUI>(ButtonWidet);
				if (_SpawnLocationButton)
				{
					AllSpawnLocationButtons.Add(_SpawnLocationButton);
				}
			}
		}

		UVerticalBox* _VerticalBox = Cast<UVerticalBox>(Box);
		if (_VerticalBox)
		{
			for (UWidget* ButtonWidet : _VerticalBox->GetAllChildren())
			{
				UKingCatcherFireButtonUI* _SpawnLocationButton = Cast<UKingCatcherFireButtonUI>(ButtonWidet);
				if (_SpawnLocationButton)
				{
					AllSpawnLocationButtons.Add(_SpawnLocationButton);
				}
			}
		}
	}
}
