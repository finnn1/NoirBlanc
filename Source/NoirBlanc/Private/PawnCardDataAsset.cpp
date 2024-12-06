// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardDataAsset.h"

void UPawnCardDataAsset::InitMeshMaterial(UStaticMeshComponent* StaticMeshComp)
{
	if(StaticMeshComp)
	{
		UMaterialInstanceDynamic* MatDynamic = UMaterialInstanceDynamic::Create(GetCardMaterialIns(), this);
		StaticMeshComp->SetMaterial(4, MatDynamic);
		FLinearColor NewUVOffsets(1.f,1.f, U_Offset,V_Offset);
		MatDynamic->SetVectorParameterValue(TEXT("UV"), NewUVOffsets);
	}
}

void UPawnCardDataAsset::SetMatchingMaterial(UStaticMeshComponent* StaticMeshComp)
{
	if(StaticMeshComp)
	{
		StaticMeshComp->SetMaterial(0, GetMatchCompleteMat());
		StaticMeshComp->SetMaterial(4, GetMatchCompleteMat());
	}
}
