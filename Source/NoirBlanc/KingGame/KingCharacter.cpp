// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KingGameMode.h"
#include "NoirBlancGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NoirBlanc/BishopGame/NoirBlancPlayerState.h"
#include "NoirBlanc/BishopGame/WaitingOtherPlayerUI.h"

class UEnhancedInputLocalPlayerSubsystem;

AKingCharacter::AKingCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;       // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment
		(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AKingCharacter::BeginPlay()
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

void AKingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KingMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKingCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKingCharacter::Look);
	}
	else
	{
		UE_LOG
		(
		 LogTemp,
		 Error,
		 TEXT(
			 "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		 ),
		 *GetNameSafe(this)
		);
	}
}

void AKingCharacter::Joined(APlayerController* NewPlayer)
{
	ServerRPC_Joined(NewPlayer);
}

void AKingCharacter::ServerRPC_Joined_Implementation(APlayerController* JoinedPlayer)
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

void AKingCharacter::MulticastRPC_ShowWaitingUI_Implementation(APlayerController* JoinedPlayer)
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

EPieceColor AKingCharacter::GetPieceColor_Implementation()
{
	return GetPlayerState<ANoirBlancPlayerState>()->PieceColor;
}

void AKingCharacter::MulticastRPC_UpdateStartCountdownUI_Implementation(const FText& NewText)
{
	if (WaitingUI) WaitingUI->RemoveFromParent();
	
	if (IsLocallyControlled())
	{
		CountDownUI->UpdateCountDown(NewText);
	}
}

void AKingCharacter::MulticastRPC_SetInput_Implementation(bool bIsEnable)
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

void AKingCharacter::MulticastRPC_InitializeMainGameUI_Implementation()
{
	if (IsLocallyControlled())
	{
		// Main UI 띄우기
		if (KingGameMainUIClass == nullptr) return;
		KingGameMainUI = CreateWidget<UKingGameMainUI>(GetWorld()->GetFirstPlayerController(), KingGameMainUIClass);
		if (KingGameMainUI == nullptr) return;
		KingGameMainUI->AddToViewport();
	}
}

void AKingCharacter::MulticastRPC_UpdateMainTimerUI_Implementation(const FText& NewText)
{
	if (IsLocallyControlled())
	{
		if (KingGameMainUI)
		{
			KingGameMainUI->Text_Timer->SetText(NewText);
		}
	}
}

void AKingCharacter::MulticastRPC_SetWinner_Implementation(EPieceColor WinnerColor)
{
	if (IsLocallyControlled())
	{
		UNoirBlancGameInstance* _NoirBlancGameInstance = GetGameInstance<UNoirBlancGameInstance>();
		if (_NoirBlancGameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tagger :: Winner : %d"), WinnerColor);
			_NoirBlancGameInstance->WinnerColor = WinnerColor;
		}
	}
}

void AKingCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKingCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
