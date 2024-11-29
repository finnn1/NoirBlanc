// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "NoirBlanc/BishopGame/BishopGameMode.h"
#include "NoirBlanc/BishopGame/NoirBlancPlayerState.h"
#include "NoirBlanc/BishopGame/TypingUIWidget.h"
#include "NoirBlanc/BishopGame/BishopWeapon.h"

class ANoirBlancPlayerState;
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		// Tagger 플레이어에게 타이핑 UI 띄우기
		if (TypingUIWidgetClass == nullptr) return;
		TypingUIWidget = CreateWidget<UTypingUIWidget>(GetWorld()->GetFirstPlayerController(), TypingUIWidgetClass);
		if (TypingUIWidget == nullptr) return;
		TypingUIWidget->AddToViewport();
		TypingUIWidget->UserInput->SetVisibility(ESlateVisibility::Hidden);
		
		// TODO: 비즈니스 로직은 서버에서 해줘야 함
		// 서버에서 랜덤 텍스트 요청하기
		ServerRPC_SetRandomText();
	}
}

void ATP_ThirdPersonCharacter::ServerRPC_SetRandomText_Implementation()
{
	// GameMode에게 RandomText 생성해 달라고 요청하기
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	if (BishopGameMode)
	{
		FText RandomText = BishopGameMode->PickRandomText();
		// 가져온 랜덤 텍스트로 UI에 세팅하기.
		MulticastRPC_SetUITextTo(FText::FromString(""), RandomText);
	}
}

void ATP_ThirdPersonCharacter::UpdateText(const FText& InputedText)
{
	ServerRPC_UpdateText(InputedText);
}

void ATP_ThirdPersonCharacter::ServerRPC_UpdateText_Implementation(const FText& InputedText)
{
	// 받은 텍스트를 모두에게 띄워야 함.
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	if (BishopGameMode == nullptr) return;

	MulticastRPC_SetUITextTo(InputedText, BishopGameMode->CurrentTextToType);
}

void ATP_ThirdPersonCharacter::MulticastRPC_SpawnWeapon_Implementation(FVector Location, FRotator Rotation, UClass* WeaponClass)
{
	// 공격하라고 Multicast
	GetWorld()->SpawnActor<ABishopWeapon>(WeaponClass, Location, Rotation);
}

void ATP_ThirdPersonCharacter::MulticastRPC_SetUITextTo_Implementation
(
	const FText& InputedText,
	const FText& NewText
)
{
	if (TypingUIWidget == nullptr) return;
	TypingUIWidget->TypedTextUI->SetText(InputedText);
	TypingUIWidget->TextToTypeUI->SetText(NewText);
}


//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATP_ThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATP_ThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}