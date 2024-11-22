// Fill out your copyright notice in the Description page of Project Settings.


#include "TypingUIWidget.h"
#include "BishopGameMode.h"
#include "BishopGamePlayerController.h"
#include "BishopPawn.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Input/Reply.h"

// TypingUIWidget.cpp

void UTypingUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

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
	UE_LOG(LogTemp, Warning, TEXT("Text Changed!! : %s"), *TypedTextUI->GetText().ToString());
	// TypedTextUI->SetText(Text);

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

FText UTypingUIWidget::ToDefaultText(FText Text)
{
	FString ClearedText = ClearTextStyle(Text);
	return FText::FromString(ClearedText);
}

FText UTypingUIWidget::ToCorrectText(FText Text)
{
	FString ClearedText = ClearTextStyle(Text);
	FString NewText = TEXT("<Correct>") + ClearedText + TEXT("</>");
	return FText::FromString(NewText);
}

FText UTypingUIWidget::ToWrongText(FText Text)
{
	FString ClearedText = ClearTextStyle(Text);
	FString NewText = TEXT("<Wrong>") + ClearedText + TEXT("</>");
	return FText::FromString(NewText);
}

FString UTypingUIWidget::ClearTextStyle(FText Text)
{
	FString NewText = Text.ToString()
	                      .Replace(TEXT("<Wrong>"), TEXT(""))
	                      .Replace(TEXT("<Correct>"), TEXT(""))
	                      .Replace(TEXT("</>"), TEXT(""));

	return NewText;
}
