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

void UPawnCardDataAsset::SetMatchingMaterial(UStaticMeshComponent* StaticMeshComp, float Alpha)
{
	if (StaticMeshComp)
	{
		// 머티리얼 인스턴스 동적 생성
		UMaterialInstanceDynamic* MaterialInstance1 = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(0);
		UMaterialInstanceDynamic* MaterialInstance2 = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(4);
		
		
		// Alpha 파라미터 값 설정
		MaterialInstance1->SetScalarParameterValue(FName("Alpha"), Alpha);
		MaterialInstance2->SetScalarParameterValue(FName("Alpha"), Alpha);
	}
}
