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
#include "Fortress/Bomb.h"
#include "Fortress/CannonBall.h"
#include "Fortress/Missile.h"
#include "Algo/RandomShuffle.h"
#include "Components/WidgetSwitcher.h"
#include "Fortress/AngleWidget.h"
#include "Runtime/AdvancedWidgets/Public/Components/RadialSlider.h"

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

	ImpulseBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("FireBoostWidget"));
	ImpulseBar->SetupAttachment(RootComponent);

	MaxHealth = 100.0f;
	Health = MaxHealth;
	// Damage = 10.0f;

	this->NetUpdateFrequency = 100.0f;

	Muzzle->SetUsingAbsoluteLocation(true);

	bIsturn = false;

	AngleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AngleWidgetComponent"));
	AngleWidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	gm = Cast<AFortressGameMode>(GetWorld()->GetAuthGameMode());

	StartLocation = GetActorLocation();
	StartRotation = Muzzle->GetComponentRotation();

	// does not work if it is located in constructor
	ProjectilesEqBasedFactoryArray.Append({
		                                      ProjectileEqBasedFactory,
		                                      CannonBallFactory, MissileFactory, BombFactory
	                                      });

	numprojectile = ProjectilesEqBasedFactoryArray.Num();

	for (int32 i = 0; i < numprojectile; i++) randomProjectileIdxArray.Add(i);

	// set Impulse bar widget
	if (ImpulseBar != nullptr)
	{
		UUserWidget* Widget = ImpulseBar->GetWidget(); // WidgetComponent != Widget
		if (Widget != nullptr)
		{
			ProjectileImpulse = 0.0f;
			FireBoostWidget = Cast<UFireBoostWidget>(Widget);
			if (FireBoostWidget != nullptr)
				FireBoostWidget->Impulse = ProjectileImpulse;
		}
	}

	if (AngleWidgetComponent != nullptr)
	{
		AngleWidget = Cast<UAngleWidget>(AngleWidgetComponent->GetWidget());
		AngleWidget->RadialSlider->SetVisibility(ESlateVisibility::Visible);
	}

	// add players in the array
	if (IsLocallyControlled())
	{
		FortressUI = CreateWidget<UFortressUI>(GetWorld(), FortressUIFactory);
		ServerRPC_NewPlayerJoined();
		if (HasAuthority()) this->bIsturn = true; // set the first player turn true
	}

	if (IsLocallyControlled() && BackgroundSound != nullptr)
		UGameplayStatics::PlaySound2D(this, BackgroundSound);
}

void ACannon::ServerRPC_NewPlayerJoined_Implementation()
{
	gm->NewPlayerJoined(this);
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
	//UE_LOG(LogTemp, Warning, TEXT("%s, %d - 1111"), *GetActorNameOrLabel(), IsLocallyControlled());
}

void ACannon::OnRep_Controller()
{
	Super::OnRep_Controller();
	//UE_LOG(LogTemp, Warning, TEXT("%s, %d - 1111"), *GetActorNameOrLabel(), IsLocallyControlled());
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//BillboardFireBoost();
	if (!bIsturn) return;
	
	FVector NewLocation = GetActorLocation() + (MovementInput * MoveSpeed * DeltaTime);

	FRotator NewRotation = Muzzle->GetComponentRotation() + RotationInput * RotationSpeed * DeltaTime;

	if (IsLocallyControlled())
		ServerRPC_Move(NewLocation, NewRotation);
}

void ACannon::ServerRPC_Move_Implementation(FVector NewLocation, FRotator NewRotation)
{
	MulticastRPC_Move(NewLocation, NewRotation);
}

void ACannon::MulticastRPC_Move_Implementation(FVector NewLocation, FRotator NewRotation)
{
	// location
	float distance = FMath::Abs(FVector::Dist(StartLocation, NewLocation));
	if (distance < DistanceRange) // if it is within the limit range, set actor location
		SetActorLocation(NewLocation);

	// rotation
	float angle = NewRotation.Roll - StartRotation.Roll;

	// blanc-white: -40~40, noir-black: 320-360, 0-40
	
	//UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *GetActorNameOrLabel(), IsLocallyControlled());
	
	if (angle >360-AngleRange) angle -= 360;
	if (angle < AngleRange && angle > -AngleRange)
	{
		Muzzle->SetRelativeRotation(FRotator(0.0f, -90.0f, NewRotation.Roll));
		if (AngleWidget) AngleWidget->RadialSlider->SetValue(0.5 + 0.5 * (angle / AngleRange));
	}
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

	if (projectileIdx == 0)
		Algo::RandomShuffle(randomProjectileIdxArray);

	if (projectileIdx >= numprojectile) projectileIdx -= numprojectile;
	//UE_LOG(LogTemp, Warning, TEXT("total num: %d"), randomProjectileIdxArray.Num());

	ProjectileEqBasedClass = ProjectilesEqBasedFactoryArray[projectileIdx];

	if (ProjectileEqBasedFactory)
		ServerRPC_Fire(ProjectileImpulse, ProjectileEqBasedClass);

	if (FireBoostWidget != nullptr)
		FireBoostWidget->Impulse = 0.0f; // set Impulse var in widget 0

	projectileIdx++;
}

void ACannon::ServerRPC_Fire_Implementation(float Impulse, TSubclassOf<AProjectileEqBased> ProjectileEqBasedSubclass)
{
	MulticastRPC_Fire(Impulse, ProjectileEqBasedSubclass);

	// after firing, switch the turn and display the turn, only server can access to the gamemode
	if (gm != nullptr)
		gm->ChangeTurn();
}

void ACannon::MulticastRPC_Fire_Implementation(float Impulse, TSubclassOf<AProjectileEqBased> ProjectileEqBasedSubclass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	ProjectileImpulse = Impulse;


	// spawn projectile
	FVector direction = SpawnLocation->GetComponentLocation() - SpawnOrigin->GetComponentLocation();

	GetWorld()->SpawnActor<AProjectileEqBased>(ProjectileEqBasedSubclass,
	                                           SpawnLocation->GetComponentLocation(), direction.Rotation(),
	                                           SpawnParams);
}

void ACannon::StartCharging(const FInputActionValue& Value)
{
	if (!bIsturn) return;

	ProjectileImpulse = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(SpeedIncreaseTimerHandle, this, &ACannon::ContinueCharging, 0.1f, true);
	if (FireBoostWidget != nullptr)
	{
		FireBoostWidget->Impulse = ProjectileImpulse;
	}
}

void ACannon::ContinueCharging()
{
	if (!bIsturn) return;

	ProjectileImpulse += ImpulseChange;

	if (FireBoostWidget != nullptr)
	{
		FireBoostWidget->Impulse = ProjectileImpulse;
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
	FVector forward = -cam->GetActorForwardVector(); // - sign to face the camera
	FVector up = cam->GetActorUpVector();
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(forward, up);

	ImpulseBar->SetWorldRotation(rot);
}

void ACannon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACannon, bIsturn);
	DOREPLIFETIME(ACannon, turnCannon);
}

void ACannon::ClientRPC_UpdateCountdownUI_Implementation(const FText& text)
{
	if (FortressUI && FortressUI->WBP_CountDownUI)
		FortressUI->WBP_CountDownUI->UpdateCountDown(text);
}

void ACannon::MulticastRPC_SetWindForce_Implementation(int32 WindStrength, float Unit)
{
	WindForce = FVector(1.0f, 0.0f, 0.0f)*Unit*WindStrength;
	
	if (IsLocallyControlled())
		FortressUI->SetWindBar(WindStrength);
}

void ACannon::ClientRPC_SwitchWidget_Implementation(int32 index)
{
	FortressUI->WidgetSwitcher->SetActiveWidgetIndex(index);
}

void ACannon::ClientRPC_AddToViewport_Implementation()
{
	FortressUI->AddToViewport();
}

void ACannon::ClientRPC_EnableInput_Implementation(bool enable)
{
	if (enable)
		EnableInput(GetWorld()->GetFirstPlayerController());
	else
		DisableInput(GetWorld()->GetFirstPlayerController());
}
