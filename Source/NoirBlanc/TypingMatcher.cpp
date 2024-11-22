// Fill out your copyright notice in the Description page of Project Settings.


#include "TypingMatcher.h"

UTypingMatcher::UTypingMatcher()
{
	
}

bool UTypingMatcher::CheckKeyIsRight(const FText PressedKey)
{
	const TCHAR* PressedChar = *PressedKey.ToString();
	return TargetString[CurrentPressedKeyIndex] == *PressedChar;
}

void UTypingMatcher::IncreaseKeyIndex()
{
CurrentPressedKeyIndex += 1;
}

bool UTypingMatcher::IsComplete() const
{
	return TargetString.Len() == CurrentPressedKeyIndex + 1;
}

void UTypingMatcher::Reset()
{
	CurrentPressedKeyIndex = 0;
}
