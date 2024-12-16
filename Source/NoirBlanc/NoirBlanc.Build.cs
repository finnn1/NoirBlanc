// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NoirBlanc : ModuleRules
{
	public NoirBlanc(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
			"GeometryCollectionEngine",
             "AdvancedWidgets"
		});

		// Uncomment if you are using Slate UI
		/*
			UMG (고수준 UI 시스템)
				↓
			Slate (중간 레벨 UI 구현)
				↓
			SlateCore (저수준 UI 코어)

			당신이 ETextCommit을 사용하려고 할 때 SlateCore가 필요한 이유는 이 열거형이 SlateCore 모듈에 정의되어 있기 때문입니다.
		*/
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore"
		});

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}