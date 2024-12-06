// Fill out your copyright notice in the Description page of Project Settings.


#include "NoirBlancGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

void UNoirBlancGameInstance::Init()
{
	Super::Init();
	
	BoardTypeData = InitTypeValue;
	BoardColorData = InitColorValue;

	for(int i = 0 ; i < 64; i++)
	{
		EncounterCountData.Add(0);
		MoveCountData.Add(0);
	}

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr) return;
	SessionInterFace = Subsystem->GetSessionInterface();

	// Delegate 등록
	SessionInterFace->OnCreateSessionCompleteDelegates.AddUObject(
		this, &UNoirBlancGameInstance::OnCreateSessionComplete);
	SessionInterFace->OnDestroySessionCompleteDelegates.AddUObject(
		this, &UNoirBlancGameInstance::OnDestroySessionComplete);
	SessionInterFace->OnFindSessionsCompleteDelegates.AddUObject(
		this, &UNoirBlancGameInstance::OnFindOtherSessionComplete);
	SessionInterFace->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UNoirBlancGameInstance::OnJoinOtherSessionComplete);
}

void UNoirBlancGameInstance::ServerRPC_LevelTravel_Implementation(const FString& LevelName)
{
	GetWorld()->ServerTravel(TEXT("/Game/Level/Lv_PawnCard"), true, true); 
}

void UNoirBlancGameInstance::CreateSession(FString DisplayName, FName _SelectedGame)
{
	SelectedGame = _SelectedGame;
	// 세션 세팅
	FOnlineSessionSettings SessionSettings;
	
	// Lan 사용 여부
	FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("Subsystem Name : %s"), *SubsystemName.ToString());
	SessionSettings.bIsLANMatch = SubsystemName.IsEqual(FName(TEXT("NULL"))); // Steam Subsystem을 불러올 수 없으면 Lan 게임으로

	// 검색 허용 여부
	SessionSettings.bShouldAdvertise = true;

	// 게임 도중 참가 가능 여부
	SessionSettings.bAllowJoinInProgress = true;

	// 세션 최대 인원
	SessionSettings.NumPublicConnections = 2;
	
	// 방 이름
	SessionSettings.Set(TEXT("DP_NAME"), DisplayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	// SessionSettings.Set(TEXT("SELECTED_GAME"), SelectedGame.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 세션 생성
	FUniqueNetIdPtr NetID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().
	                                    GetUniqueNetId();
	SessionInterFace->CreateSession(*NetID, FName(DisplayName), SessionSettings);
}

void UNoirBlancGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *SessionName.ToString());
		// 세션 만든 사람(서버) 이 만들어진 세션으로 이동
		FString ServerURL;
		ServerURL = *GameLevelMap.Find(SelectedGame);
		if (ServerURL.IsEmpty() == false)
		{
			ServerURL += TEXT("?listen");
			GetWorld()->ServerTravel(ServerURL);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 실패"), *SessionName.ToString());
	}
}

void UNoirBlancGameInstance::DestroySession(FString SessionName)
{
	SessionInterFace->DestroySession(FName(SessionName));
}

void UNoirBlancGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 파괴 성공"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 파괴 실패"), *SessionName.ToString());
	}
}

void UNoirBlancGameInstance::FindOtherSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션 검색 시작"));
	OnFindComplete.ExecuteIfBound(false);

	// 세션 검색 설정
	SessionSearchResult = MakeShared<FOnlineSessionSearch>();

	// 랜을 사용할지 여부
	FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름 : %s"), *SubsystemName.ToString());
	SessionSearchResult->bIsLanQuery = SubsystemName.IsEqual(FName(TEXT("NULL")));

	// 활성화 되어있는 세션만 검색하자.
	SessionSearchResult->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//sessionSearch->QuerySettings.Set(TEXT("DP_NAME"), FString(TEXT("SeSAC1")), EOnlineComparisonOp::Equals);

	// 세션을 몇개까지 검색할지
	SessionSearchResult->MaxSearchResults = 100;

	// 세션 검색하자
	SessionInterFace->FindSessions(0, SessionSearchResult.ToSharedRef());
}

void UNoirBlancGameInstance::OnFindOtherSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> Results = SessionSearchResult->SearchResults;

		for (int32 i = 0; i < Results.Num(); i++)
		{
			FOnlineSessionSearchResult _SessionResult = Results[i];

			// DP_NAME 가져오자
			FString DisplayName;
			_SessionResult.Session.SessionSettings.Get(TEXT("DP_NAME"), DisplayName);

			// 세션을 만들 사람 이름
			FString SessionCreator = _SessionResult.Session.OwningUserName;
			UE_LOG(LogTemp, Warning, TEXT("세션 : %s, 만든이 : %s"), *DisplayName, *SessionCreator);

			// 세션 정보를 넘겨서 SessionItem 을 추가하게 하자.
			FString SessionInfo = FString::Printf(TEXT("%s - %s"), *DisplayName, *SessionCreator);
			OnAddSession.ExecuteIfBound(i, SessionInfo);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("세션 검색 완료"));
	OnFindComplete.ExecuteIfBound(true);
}

void UNoirBlancGameInstance::JoinOtherSession(int32 Index)
{
	auto Results = SessionSearchResult->SearchResults;

	// 세션 이름
	FString DisplayName;
	Results[Index].Session.SessionSettings.Get(TEXT("DP_NAME"), DisplayName);

	// 세션 참여
	SessionInterFace->JoinSession(0, FName(DisplayName), Results[Index]);
}

void UNoirBlancGameInstance::OnJoinOtherSessionComplete(FName SessionName,
                                                            EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString url;
		SessionInterFace->GetResolvedConnectString(SessionName, url);

		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
	}
}