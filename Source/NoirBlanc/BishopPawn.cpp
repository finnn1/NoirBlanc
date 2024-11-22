// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopPawn.h"
#include "BishopGameMode.h"
#include "TypingUIWidget.h"
#include "Weapon.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABishopPawn::ABishopPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComponent;

	DebugText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DebugText"));
	DebugText->SetupAttachment(RootComponent);

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

// Called when the game starts or when spawned
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

// Called every frame
void ABishopPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// DebugText->SetWorldScale3D(FVector(3.f));
	// if (GetPlayerState<ANoirBlancPlayerState>())
	// {
	// 	DebugText->SetText(FText::FromString(GetPlayerState()->GetActorNameOrLabel()));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("GetPlayerState<ANoirBlancPlayerState Is not exist"));
	// }
}

// Called to bind functionality to input
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
		FText RandomText = BishopGameMode->PickRandomText();
		// 가져온 랜덤 텍스트로 UI에 세팅하기.
		MulticastRPC_SetUITextTo(FText::FromString(""), RandomText);
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
	GetWorld()->SpawnActor<AWeapon>(WeaponClass, Location, Rotation);
}

void ABishopPawn::MulticastRPC_SetUITextTo_Implementation
(
	const FText& InputedText,
	const FText& NewText
)
{
	if (TypingUIWidget == nullptr) return;
	TypingUIWidget->TypedTextUI->SetText(InputedText);
	TypingUIWidget->TextToTypeUI->SetText(NewText);
	TypingUIWidget->UserInput->SetText(InputedText);
}
