// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingCatcherPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KingCatcherUI.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "KingCatcherUI.h"
#include "KingCharacter.h"
#include "KingGameMode.h"
#include "NoirBlancGameInstance.h"
#include "SpawnLocation.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "NoirBlanc/BishopGame/NoirBlancPlayerState.h"

class UEnhancedInputLocalPlayerSubsystem;

AKingCatcherPawn::AKingCatcherPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(SphereComponent);
	CameraBoom->TargetArmLength = 400.0f;
	// CameraBoom->bUsePawnControlRotation = true;

	CameraBoom->SetRelativeRotation(FRotator(-90, 0, 0));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AKingCatcherPawn::OnConfirmButtonClicked()
{
	ServerRPC_OnConfirmButtonClicked();
}

void AKingCatcherPawn::ServerRPC_OnConfirmButtonClicked_Implementation()
{
	// TODO: 서버에게 클릭한 SpawnLocation 보내서 처리하도록 하기
	// 1. 클라이언트 Pawn 쪽에서 클릭 했다는 사실을 서버 Pawn에게 알림
	// 2. 서버 Pawn에서 클릭 트레이싱을 해서 FHitResult를 구한다.
	// 3. 서버 Pawn에서 어떤 SpawnLocation이 맞았는지 배열로 쌓아놓는다.
	// 4. 4개 이상 쌓이면 더 이상 못 쌓게 하기.

	// 화면의 Go! 버튼을 누르면 해당 위치에서 레이저 발사!
	// 서버 Pawn 쪽에서 갯수 4개가 차면 서버 KingGameMode로 보내서 처리하도록 하기!

	// if (SelectedSpawnLocations.Num() < 4) return;
	for (class ASpawnLocation* SelectedSpawnLocation : SelectedSpawnLocations)
	{
		MulticastRPC_SelectForAll(SelectedSpawnLocation);
	}

	MulticastRPC_SetInput(false);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
		(
		 TimerHandle,
		 FTimerDelegate::CreateLambda
		 (
		  [this]()
		  {
			  UE_LOG(LogTemp, Warning, TEXT("1초 지남!"));
			  for (class ASpawnLocation* SelectedSpawnLocation : this->SelectedSpawnLocations)
			  {
				  this->MulticastRPC_DeselectForAll(SelectedSpawnLocation);
			  }

			  AKingGameMode* KingGameMode = Cast<AKingGameMode>(GetWorld()->GetAuthGameMode());
			  if (KingGameMode)
			  {
				  // TODO: 게임모드에서 로직 처리
				  KingGameMode->FireAt(SelectedSpawnLocations);
			  }

			  SelectedSpawnLocations.Empty();
			  MulticastRPC_SetInput(true);
		  }
		 ),
		 1.f,
		 false
		);
}

void AKingCatcherPawn::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}

	// 로컬컨트롤 하고 있는 애라면? --> 클라니까 서버한테 Join 했다고 알려주자.
	if (IsLocallyControlled())
	{
		APlayerController* NewPlayerController = Cast<APlayerController>(GetController());
		if (NewPlayerController)
		{
			Joined(NewPlayerController);
			UE_LOG(LogTemp, Warning, TEXT("Tagger Character Possed : %s"), *GetActorNameOrLabel());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tagger Character PlayerController is NULL!!"));
		}
	}
}

void AKingCatcherPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKingCatcherPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CatcherMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &AKingCatcherPawn::Click);
	}
	else
	{
		UE_LOG
		(
		 LogTemp,
		 Error,
		 TEXT("'%s'Failed to find an Enhanced Input component! "
			 "This template is built to use the Enhanced Input system. "
			 "If you intend to use the legacy system, then you will need to update this C++ file."),
		 *GetNameSafe(this)
		);
	}
}

void AKingCatcherPawn::Joined(APlayerController* NewPlayer)
{
	ServerRPC_Joined(NewPlayer);
}

void AKingCatcherPawn::ServerRPC_Joined_Implementation(APlayerController* JoinedPlayer)
{
	// 지금 들어온 플레이어 입력 방지
	MulticastRPC_SetInput(false);
	MulticastRPC_ShowWaitingUI(JoinedPlayer);

	// 게임모드 JoinedPlayers 배열에 플레이어 추가
	AKingGameMode* _KingGameMode = GetWorld()->GetAuthGameMode<AKingGameMode>();
	if (_KingGameMode)
	{
		_KingGameMode->NotifyJoined(JoinedPlayer);
	}
}

void AKingCatcherPawn::MulticastRPC_ShowWaitingUI_Implementation(APlayerController* JoinedPlayer)
{
	// Waiting For Player UI 띄우기
	if (IsLocallyControlled())
	{
		WaitingUI = CreateWidget<UWaitingUI>
			(
			 JoinedPlayer,
			 WaitingUIClass
			);
		if (WaitingUI)
		{
			WaitingUI->AddToViewport();
		}

		CountDownUI = CreateWidget<UCountDownUI>
			(
			 JoinedPlayer,
			 CountDownUIClass
			);
		if (CountDownUI)
		{
			CountDownUI->AddToViewport();
			CountDownUI->Txt_Count->SetText(FText());
		}
	}
}

EPieceColor AKingCatcherPawn::GetPieceColor_Implementation()
{
	return GetPlayerState<ANoirBlancPlayerState>()->PieceColor;
}

void AKingCatcherPawn::MulticastRPC_UpdateStartCountdownUI_Implementation(const FText& NewText)
{
	if (WaitingUI) WaitingUI->RemoveFromParent();

	if (IsLocallyControlled())
	{
		CountDownUI->Txt_Count->SetText(NewText);
	}
}

void AKingCatcherPawn::MulticastRPC_SetInput_Implementation(bool bIsEnable)
{
	if (IsLocallyControlled())
	{
		if (bIsEnable)
		{
			EnableInput(GetWorld()->GetFirstPlayerController());
		}
		else
		{
			DisableInput(GetWorld()->GetFirstPlayerController());
		}
	}
}

void AKingCatcherPawn::MulticastRPC_InitializeMainGameUI_Implementation()
{
	if (IsLocallyControlled())
	{
		// Main UI 띄우기
		if (KingGameMainUIClass == nullptr) return;
		KingGameMainUI = CreateWidget<UKingGameMainUI>(GetWorld()->GetFirstPlayerController(), KingGameMainUIClass);
		if (KingGameMainUI == nullptr) return;
		KingGameMainUI->AddToViewport();

		// Catcher 플레이어에게 UI 띄우기
		if (CatcherUIClass == nullptr) return;
		CatcherUI = CreateWidget<UKingCatcherUI>(GetWorld()->GetFirstPlayerController(), CatcherUIClass);
		if (CatcherUI == nullptr) return;
		CatcherUI->AddToViewport();
		CatcherUI->ConfirmButton->OnClicked.AddDynamic(this, &AKingCatcherPawn::OnConfirmButtonClicked);

		APlayerController* _PlayerController = GetWorld()->GetFirstPlayerController();
		if (_PlayerController)
		{
			_PlayerController->SetShowMouseCursor(true);
		}
	}
}

void AKingCatcherPawn::MulticastRPC_UpdateMainTimerUI_Implementation(const FText& NewText)
{
	if (IsLocallyControlled())
	{
		if (KingGameMainUI)
		{
			KingGameMainUI->Text_Timer->SetText(NewText);
		}
	}
}

void AKingCatcherPawn::MulticastRPC_SetWinner_Implementation(EPieceColor WinnerColor)
{
	if (IsLocallyControlled())
	{
		UNoirBlancGameInstance* _NoirBlancGameInstance = GetGameInstance<UNoirBlancGameInstance>();
		if (_NoirBlancGameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bishop :: Winner : %d"), WinnerColor);
			_NoirBlancGameInstance->WinnerColor = WinnerColor;
		}
	}
}

void AKingCatcherPawn::Click(const struct FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Catcher Clicked!!"));

	// 마우스로 클릭된 위치 가져오기
	APlayerController* _PlayerContoller = Cast<APlayerController>(GetController());
	if (_PlayerContoller == nullptr) return;

	FVector WorldLocation;
	FVector WorldDirection;
	bool _bIsDeprojectSuccess = _PlayerContoller->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	if (_bIsDeprojectSuccess == false) return;

	ServerRPC_Click(WorldLocation, WorldDirection);
}

void AKingCatcherPawn::MulticastRPC_SelectForAll_Implementation(ASpawnLocation* SpawnLocation)
{
	if (HasAuthority())
	{
		SpawnLocation->bIsSelected = true;
	}

	SpawnLocation->ColorToRed();
}

void AKingCatcherPawn::MulticastRPC_DeselectForAll_Implementation(ASpawnLocation* SpawnLocation)
{
	if (HasAuthority())
	{
		SpawnLocation->bIsSelected = false;
	}

	SpawnLocation->ColorToWhite();
}

void AKingCatcherPawn::ServerRPC_Click_Implementation(FVector WorldLocation, FVector WorldDirection)
{
	// 서버에서 라인 트레이스 수행
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	float TraceDistance = 10000.0f;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	if (GetWorld()->LineTraceSingleByChannel
		(
		 HitResult,
		 WorldLocation,
		 WorldLocation + (WorldDirection * TraceDistance),
		 TraceChannel,
		 QueryParams
		))
	{
		// 히트 결과 처리
		UE_LOG(LogTemp, Warning, TEXT("Clicked object : %s"), *HitResult.GetActor()->GetActorNameOrLabel());
	}

	for (int32 i = 0; i < SelectedSpawnLocations.Num(); i++)
	{
		UE_LOG
		(
		 LogTemp,
		 Warning,
		 TEXT("Selected SpawnLocation : %s"),
		 *SelectedSpawnLocations[i]->GetActorNameOrLabel()
		);
	}

	ASpawnLocation* SpawnLocation = Cast<ASpawnLocation>(HitResult.GetActor());
	if (SpawnLocation)
	{
		if (SpawnLocation->bIsSelected == false)
		{
			// 4개 이상 선택하려 하면 return 시키자!
			if (SelectedSpawnLocations.Num() >= 4) return;

			SelectedSpawnLocations.Add(SpawnLocation);

			// 누른 사람에게만 빨간색으로 보이게 하기!
			MulticastRPC_SelectOnlyForLocallyPlayer(SpawnLocation);
		}
		else
		{
			SelectedSpawnLocations.Remove(SpawnLocation);
			MulticastRPC_DeselectOnlyForLocallyPlayer(SpawnLocation);
		}
	}
}

void AKingCatcherPawn::MulticastRPC_SelectOnlyForLocallyPlayer_Implementation(ASpawnLocation* SpawnLocation)
{
	// 선택 됐는지 여부는 서버에 저장.
	if (HasAuthority())
	{
		SpawnLocation->bIsSelected = true;
	}

	// 색상은 Local Player가 Catcher Pawn인 경우에만 변경
	// (1p가 Catcher Pawn이라는 뜻)
	if (IsLocallyControlled() == false) return;
	SpawnLocation->ColorToRed();
}

void AKingCatcherPawn::MulticastRPC_DeselectOnlyForLocallyPlayer_Implementation(ASpawnLocation* SpawnLocation)
{
	// 선택 됐는지 여부는 서버에 저장.
	if (HasAuthority())
	{
		SpawnLocation->bIsSelected = false;
	}

	// 색상은 Local Player가 Catcher Pawn인 경우에만 변경
	// (1p가 Catcher Pawn이라는 뜻)
	if (IsLocallyControlled() == false) return;
	SpawnLocation->ColorToWhite();
}
