// Fill out your copyright notice in the Description page of Project Settings.


#include "TypingUIWidget.h"
#include "BishopPawn.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Input/Reply.h"

// TypingUIWidget.cpp

void UTypingUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextToTypeUI->SetAutoWrapText(true);
	TextToTypeUI->SetJustification(ETextJustify::Type::Center);

	TypedTextUI->SetAutoWrapText(true);
	TypedTextUI->SetJustification(ETextJustify::Type::Center);

	if (UserInput)
	{
		UserInput->OnTextChanged.AddDynamic(this, &UTypingUIWidget::OnTextChanged);
		UserInput->OnTextCommitted.AddDynamic(this, &UTypingUIWidget::OnTextCommitted);

		UserInput->SetKeyboardFocus();
		UserInput->SetClearKeyboardFocusOnCommit(false);
	}
}

void UTypingUIWidget::OnTextChanged(const FText& Text)
{
	// BishopGamePlayerController에게 텍스트가 변경되고 있다고 알림
	ABishopPawn* BishopPawn = Cast<ABishopPawn>(GetOwningPlayerPawn());
	if (BishopPawn == nullptr) return;

	BishopPawn->UpdateText(Text);
}

void UTypingUIWidget::OnTextCommitted(const FText& Text, ETextCommit::Type InCommitType)
{
	if (InCommitType != ETextCommit::Type::OnEnter) return;

	UE_LOG(LogTemp, Warning, TEXT("Text Committed"));

	ABishopPawn* BishopPawn = Cast<ABishopPawn>(GetOwningPlayerPawn());
	if (BishopPawn == nullptr) return;

	BishopPawn->CommitText(Text);
}

void UTypingUIWidget::SetStyledTextToTypeUI(FText TextToType, TArray<bool> StringCorrectArray)
{
	// 맞고 틀린 배열로 클라이언트 쪽 위젯에서 업데이트 하도록 하기
	FString FormattedString;

	for (int i = 0; i < TextToType.ToString().Len(); i++)
	{
		if (i < StringCorrectArray.Num())
		{
			if (StringCorrectArray[i] == true)
			{
				FormattedString += ToCorrectText(TextToType.ToString()[i]);
			}
			else
			{
				FormattedString += ToWrongText(TextToType.ToString()[i]);
			}
		}
		else
		{
			FormattedString += TextToType.ToString()[i];
		}
	}

	TextToTypeUI->SetText(FText::FromString(FormattedString));
}


FString UTypingUIWidget::ToCorrectText(TCHAR Character)
{
	FString _String = FString(1, &Character);
	FString NewText = TEXT("<Correct>") + _String + TEXT("</>");
	return NewText;
}

FString UTypingUIWidget::ToWrongText(TCHAR Character)
{
	FString _String = FString(1, &Character);
	FString NewText = TEXT("<Wrong>") + _String + TEXT("</>");
	return NewText;
}
