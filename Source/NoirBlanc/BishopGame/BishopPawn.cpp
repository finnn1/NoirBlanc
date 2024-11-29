// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopPawn.h"
#include "BishopGameMode.h"
#include "TypingUIWidget.h"
#include "BishopWeapon.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay - HasAuthority: %s"),
	       HasAuthority() ? TEXT("True") : TEXT("False"));

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

void ABishopPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

void ABishopPawn::ServerRPC_CommitText_Implementation(const FText& TypedText)
{
	// 받은 텍스트를 모두에게 띄워야 함.
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	if (BishopGameMode == nullptr) return;

	BishopGameMode->CommitText(TypedText);
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
