// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardDataAsset.h"

void UPawnCardDataAsset::InitMeshMaterial(UStaticMeshComponent* StaticMeshComp)
{
	if(StaticMeshComp)
	{
		UMaterialInstanceDynamic* MatDynamic = UMaterialInstanceDynamic::Create(GetCardMaterialIns(), this);
		StaticMeshComp->SetMaterial(4, MatDynamic);
		FLinearColor NewUVOffsets(0.76f,1.f, U_Offset,V_Offset);
		MatDynamic->SetVectorParameterValue(TEXT("UV"), NewUVOffsets);
	}
}

void UPawnCardDataAsset::ChangeMatToBlack(UStaticMeshComponent* StaticMeshComp, int32 ChangeIndex, UMaterialInstance* BlackMatInst)
{
	if(StaticMeshComp)
	{
		StaticMeshComp->SetMaterial(ChangeIndex, BlackMatInst);
	}
}


void UPawnCardDataAsset::SetMatchingMat_ChaosDest(UStaticMeshComponent* StaticMeshComp, float Alpha)
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

void UPawnCardDataAsset::SetMatchingMat_Dissolve(UStaticMeshComponent* StaticMeshComp, float Alpha)
{
	if (StaticMeshComp)
	{
		// 머티리얼 인스턴스 동적 생성
		UMaterialInstanceDynamic* MaterialInstance = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(4);
		for(int32 i = 0; i < StaticMeshComp->GetNumMaterials(); i++)
		{
			StaticMeshComp->SetMaterial(i, MaterialInstance);

			// Alpha 파라미터 값 설정
			UMaterialInstanceDynamic* ChangeMat = StaticMeshComp->CreateAndSetMaterialInstanceDynamic(i);
			ChangeMat->SetScalarParameterValue(FName("Dissolve"), Alpha);
		}
	}
}
