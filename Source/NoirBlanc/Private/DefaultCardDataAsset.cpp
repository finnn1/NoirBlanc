// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCardDataAsset.h"

UMaterialInstance* UDefaultCardDataAsset::GetCardMaterialIns()
{
	return CardMaterialIns;
}

UMaterial* UDefaultCardDataAsset::GetMatchCompleteMat()
{
	return MatchCompleteMat;
}
