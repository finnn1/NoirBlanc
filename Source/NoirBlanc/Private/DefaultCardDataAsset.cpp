// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCardDataAsset.h"

void UDefaultCardDataAsset::NativeConstruct()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> MatInsFactory(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Asset/PawnCardAsset/FrontIMG_Inst.FrontIMG_Inst'"));
	if(MatInsFactory.Succeeded())
	{
		CardMaterialIns = MatInsFactory.Object;
	}
}

UMaterialInstance* UDefaultCardDataAsset::GetCardMaterialIns()
{
	return CardMaterialIns;
}

/*UMaterial* UDefaultCardDataAsset::GetMatchCompleteMat()
{
	return MatchCompleteMat;
}*/
