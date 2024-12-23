// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopPawn.h"
#include "BishopGameMode.h"
#include "TypingUIWidget.h"
#include "BishopWeapon.h"
#include "NoirBlancGameInstance.h"
#include "NoirBlancPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/SpringArmComponent.h"
#include "PieceTypes.h"
#include "NoirBlanc/Knight/CountDownUI.h"
#include "NoirBlanc/Knight/FinishUI.h"
#include "NoirBlanc/Knight/WaitingUI.h"


ABishopPawn::ABishopPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	WeaponSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	WeaponSpawnPoint->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	SpringArmComponent->SetRelativeLocation(FVector(10.f, -93.f, 100.f));
	SpringArmComponent->SetRelativeRotation(FRotator(-20, 10, 0));
	SpringArmComponent->TargetArmLength = 500.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ABishopPawn::BeginPlay()
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

void ABishopPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABishopPawn::Joined(APlayerController* NewPlayer)
{
	ServerRPC_Joined(NewPlayer);
}

void ABishopPawn::ServerRPC_Joined_Implementation(APlayerController* JoinedPlayer)
{
	// 지금 들어온 플레이어 입력 방지
	MulticastRPC_SetInput(false);
	MulticastRPC_ShowWaitingUI(JoinedPlayer);

	// 게임모드 JoinedPlayers 배열에 플레이어 추가
	ABishopGameMode* _BishopGameMode = GetWorld()->GetAuthGameMode<ABishopGameMode>();
	if (_BishopGameMode)
	{
		_BishopGameMode->NotifyJoined(JoinedPlayer);
	}
}

void ABishopPawn::MulticastRPC_ShowWaitingUI_Implementation(APlayerController* JoinedPlayer)
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

void ABishopPawn::MulticastRPC_SetInput_Implementation(bool bIsEnable)
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

void ABishopPawn::MulticastRPC_SetPieceColor_Implementation(EPieceColor NewPieceColor)
{
	GetPlayerState<ANoirBlancPlayerState>()->PieceColor = NewPieceColor;
}

EPieceColor ABishopPawn::GetPieceColor_Implementation()
{
	return GetPlayerState<ANoirBlancPlayerState>()->PieceColor;
}

void ABishopPawn::ServerRPC_SetRandomText_Implementation()
{
	// GameMode에게 RandomText 생성해 달라고 요청하기
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	if (BishopGameMode)
	{
		BishopGameMode->PickRandomTextAndUpdateUI();
	}
}

void ABishopPawn::MulticastRPC_SetWinner_Implementation(EPieceColor WinnerColor)
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

void ABishopPawn::UpdateText(const FText& InputedText)
{
	ServerRPC_UpdateText(InputedText);
}

void ABishopPawn::ServerRPC_UpdateText_Implementation(const FText& InputedText)
{
	// 받은 텍스트를 모두에게 띄워야 함.
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	if (BishopGameMode == nullptr) return;

	BishopGameMode->UpdateInputtedText(InputedText);
}

void ABishopPawn::CommitText(const FText& TypedText)
{
	ServerRPC_CommitText(TypedText);
}

void ABishopPawn::MulticastRPC_UpdateMainTimerUI_Implementation(const FText& NewText)
{
	if (IsLocallyControlled())
	{
		TypingUIWidget->Text_Timer->SetText(NewText);
	}
}

void ABishopPawn::MulticastRPC_ShowGameOverUI_Implementation(const FText& Winner)
{
	if (IsLocallyControlled())
	{
		if (IsValid(WaitingUI))
		{
			WaitingUI->RemoveFromParent();
		}

		if (IsValid(CountDownUI))
		{
			CountDownUI->RemoveFromParent();
		}

		if (IsValid(TypingUIWidget))
		{
			TypingUIWidget->RemoveFromParent();
		}

		FTimerHandle GameOverTimerHandle;
		GetWorld()->GetTimerManager().SetTimer
			(GameOverTimerHandle, [this, Winner]()
			 {
				 if (FinishUIClass)
				 {
					 FinishUI = CreateWidget<UFinishUI>(GetWorld()->GetFirstPlayerController(), FinishUIClass);
					 if (FinishUI)
					 {
						 FinishUI->AddToViewport();
						 FinishUI->UpdateWinnerText(Winner);
					 }
				 }
			 },
			 1.f,
			 false
			);
	}
}

void ABishopPawn::MulticastRPC_UpdateStartCountdownUI_Implementation(const FText& NewText)
{
	if (IsValid(WaitingUI))
	{
		WaitingUI->Txt_Waiting->SetText(FText());
	}

	if (IsLocallyControlled())
	{
		CountDownUI->UpdateCountDown(NewText);
	}
}

void ABishopPawn::ServerRPC_CommitText_Implementation(const FText& TypedText)
{
	// 받은 텍스트를 모두에게 띄워야 함.
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	if (BishopGameMode == nullptr) return;

	BishopGameMode->CommitText(TypedText);
}

void ABishopPawn::MulticastRPC_InitializeTypingUI_Implementation()
{
	if (IsLocallyControlled())
	{
		// Bishop 플레이어에게 타이핑 UI 띄우기
		if (TypingUIWidgetClass == nullptr) return;
		TypingUIWidget = CreateWidget<UTypingUIWidget>(GetWorld()->GetFirstPlayerController(), TypingUIWidgetClass);
		if (TypingUIWidget == nullptr) return;
		TypingUIWidget->AddToViewport();

		APlayerController* _PlayerController = GetWorld()->GetFirstPlayerController();
		if (_PlayerController)
		{
			_PlayerController->SetInputMode(FInputModeUIOnly());
			_PlayerController->bShowMouseCursor = true;
		}

		// TODO: 비즈니스 로직은 서버에서 해줘야 함
		// 서버에서 랜덤 텍스트 요청하기
		ServerRPC_SetRandomText();
	}
}

void ABishopPawn::MulticastRPC_SpawnWeapon_Implementation(FVector Location, FRotator Rotation, UClass* WeaponClass)
{
	// 공격하라고 Multicast
	GetWorld()->SpawnActor<ABishopWeapon>(WeaponClass, Location, Rotation);
}

void ABishopPawn::MulticastRPC_SetUITextTo_Implementation
(
	const FText& InputedText,
	const FText& CurrentTextToType,
	const TArray<bool>& StringCorrectArray
)
{
	if (TypingUIWidget == nullptr) return;
	TypingUIWidget->SetStyledTextToTypeUI(CurrentTextToType, StringCorrectArray);
	TypingUIWidget->TypedTextUI->SetText(InputedText);
	TypingUIWidget->UserInput->SetText(InputedText);
}
