// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/Cannon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Fortress/Projectile.h"
#include "Fortress/ProjectileEqBased.h"
#include "TimerManager.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Fortress/FireBoostWidget.h"
#include "Fortress/FortressGameMode.h"
#include "Fortress/FortressUI.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACannon::ACannon()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);
	Muzzle->SetMobility(EComponentMobility::Movable);

	SpawnOrigin = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnOrigin"));
	SpawnOrigin->SetupAttachment(Muzzle);

	SpawnLocation = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(Muzzle);

	MovementInput = FVector::ZeroVector;
	RotationInput = FRotator::ZeroRotator;

	MoveSpeed = 100.0f;
	RotationSpeed = 50.0f;

	VelocityBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("FireBoostWidget"));
	VelocityBar->SetupAttachment(RootComponent);

	MaxHealth = 100.0f;
	Health = MaxHealth;
	Damage = 10.0f;

	this->NetUpdateFrequency = 100.0f;

	Muzzle->SetUsingAbsoluteLocation(true);

	bIsturn = false;
	
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	// set velocity bar widget
	if (VelocityBar != nullptr)
	{
		UUserWidget* Widget = VelocityBar->GetWidget(); // WidgetComponent != Widget
		if (Widget != nullptr)
		{
			ProjectileVelocity = 0.0f;
			FireBoostWidget = Cast<UFireBoostWidget>(Widget);
			if (FireBoostWidget != nullptr)
				FireBoostWidget->Velocity = ProjectileVelocity;
		}
	}

	// add players in the array
	AFortressGameMode* gm = Cast<AFortressGameMode>(GetWorld()->GetAuthGameMode());
	if (gm != nullptr)
	{
		gm->AllPlayers.Add(this);
		if (gm->AllPlayers.Num() == 1) this->bIsturn = true;		// set the first player turn true
	}
}



// Called to bind functionality to input
void ACannon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACannon::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACannon::Move);
		EnhancedInputComponent->BindAction(ProjectileDirAction, ETriggerEvent::Triggered, this,
		                                   &ACannon::ProjectileDir);
		EnhancedInputComponent->BindAction(ProjectileDirAction, ETriggerEvent::Completed, this,
		                                   &ACannon::ProjectileDir);
		EnhancedInputComponent->BindAction(ForceAction, ETriggerEvent::Triggered, this, &ACannon::Force);
		EnhancedInputComponent->BindAction(FireBoostAction, ETriggerEvent::Started, this, &ACannon::StartCharging);
		EnhancedInputComponent->BindAction(FireBoostAction, ETriggerEvent::Completed, this, &ACannon::Fire);
	}
}

void ACannon::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("Possessed by: %s"), *GetActorNameOrLabel());

	ClientRPC_Init();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BillboardFireBoost();
	if (!bIsturn) return; 
	
	FVector NewLocation = GetActorLocation() + (MovementInput * MoveSpeed * DeltaTime);

	// TODO: should set the limit of the rotation angle
	FRotator NewRotation = Muzzle->GetComponentRotation() + RotationInput * RotationSpeed * DeltaTime;
	
	if (IsLocallyControlled())
		ServerRPC_Move(NewLocation, NewRotation);
}

void ACannon::ServerRPC_Move_Implementation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocation(NewLocation);
	Muzzle->SetRelativeRotation(FRotator(0.0f, -90.0f, NewRotation.Roll));
}

void ACannon::Move(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move called %s"), *Value.ToString());
	if (HasAuthority()) MovementInput.X = Value.Get<float>();
	else MovementInput.X = -Value.Get<float>();
}

void ACannon::ProjectileDir(const FInputActionValue& Value)
{
	RotationInput.Yaw = 0.0f;
	RotationInput.Pitch = 0.0f;
	RotationInput.Roll = Value.Get<float>();
}

void ACannon::Fire()
{
	if (!bIsturn) return;
	
	GetWorld()->GetTimerManager().ClearTimer(SpeedIncreaseTimerHandle);

	if (ProjectileEqBasedFactory)
		ServerRPC_Fire(ProjectileVelocity);

	if (FireBoostWidget != nullptr)
		FireBoostWidget->Velocity = 0.0f; // set velocity var in widget 0
	
}

void ACannon::ServerRPC_Fire_Implementation(float Velocity)
{
	MulticastRPC_Fire(Velocity);

	// after firing, switch the turn and display the turn
	// only server can access to the gamemode
	AFortressGameMode* gm = Cast<AFortressGameMode>(GetWorld()->GetAuthGameMode());
	if (gm != nullptr)
	{
		gm->ChangeTurn();
	}
	UE_LOG(LogTemp, Warning, TEXT("Cannon turnCannon %s"), *turnCannon.ToString());
}

void ACannon::MulticastRPC_Fire_Implementation(float Velocity) 
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	this->ProjectileVelocity = Velocity;
	
	// spawn projectile
	ProjectileEqBased = GetWorld()->SpawnActor<AProjectileEqBased>(ProjectileEqBasedFactory,
																   SpawnLocation->GetComponentLocation(),
																   SpawnLocation->GetComponentRotation()+FRotator(0,90,0), SpawnParams);
}

void ACannon::StartCharging(const FInputActionValue& Value)
{
	if (!bIsturn) return; 
	
	ProjectileVelocity = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(SpeedIncreaseTimerHandle, this, &ACannon::ContinueCharging, 0.1f, true);
	if (FireBoostWidget != nullptr)
	{
		FireBoostWidget->Velocity = ProjectileVelocity;
	}
}

void ACannon::ContinueCharging()
{
	if (!bIsturn) return;
	
	ProjectileVelocity += VelocityChange;

	if (FireBoostWidget != nullptr)
	{
		FireBoostWidget->Velocity = ProjectileVelocity;
	}
}


/*void ACannon::Fire(const FInputActionValue& Value)
{
	if (ProjectileFactory)
	{
		FRotator ProjectileRotation = Muzzle->GetComponentRotation();
		FVector RotatedVector = ProjectileRotation.Vector();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileFactory,
			SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), SpawnParams);
	}
	// CreateDefaultSubobject is for generating component
}*/

void ACannon::Force(const FInputActionValue& Value)
{
}

void ACannon::BillboardFireBoost()
{
	AActor* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector forward = -cam->GetActorForwardVector();	// - sign to face the camera
	FVector up = cam->GetActorUpVector();
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(forward, up);

	VelocityBar->SetWorldRotation(rot);
}

void ACannon::InitMainUIWiget()
{
	if (IsLocallyControlled() == false) return;

	FortressUI = CreateWidget<UFortressUI>(GetWorld(), FortressUIFactory);
	if (FortressUI != nullptr)
		FortressUI->AddToViewport();
}

void ACannon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACannon, bIsturn);
	DOREPLIFETIME(ACannon, turnCannon);
}

void ACannon::ClientRPC_Init_Implementation()
{
	InitMainUIWiget();
}
