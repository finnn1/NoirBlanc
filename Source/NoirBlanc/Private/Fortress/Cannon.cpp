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
#include "Components/WidgetComponent.h"
#include "Fortress/FireBoostWidget.h"
#include "Fortress/FortressUI.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

	MovementInput =  FVector::ZeroVector;
	RotationInput = FRotator::ZeroRotator;
	
	MoveSpeed = 100.0f;
	RotationSpeed = 50.0f;

	VelocityBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("FireBoostWidget"));
	VelocityBar->SetupAttachment(RootComponent);

	MaxHealth = 100.0f;
	Health =  MaxHealth;
	Damage = 10.0f;
	
 }

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	if (VelocityBar != nullptr)
	{
		UUserWidget* Widget = VelocityBar->GetWidget();		// WidgetComponent != Widget
		if (Widget != nullptr)
		{
			ProjectileVelocity = 0.0f;
			FireBoostWidget = Cast<UFireBoostWidget>(Widget);
			if (FireBoostWidget != nullptr)
				FireBoostWidget->Velocity = ProjectileVelocity;
		}
	}
	FortressUI = CreateWidget<UFortressUI>(GetWorld(), FortressUIFactory);
	if (FortressUI != nullptr)
		FortressUI->AddToViewport();
}

// Called to bind functionality to input
void ACannon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (UEnhancedInputLocalPlayerSubsystem*  Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACannon::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACannon::Move);
		EnhancedInputComponent->BindAction(ProjectileDirAction, ETriggerEvent::Triggered, this, &ACannon::ProjectileDir);
		EnhancedInputComponent->BindAction(ProjectileDirAction, ETriggerEvent::Completed, this, &ACannon::ProjectileDir);
		EnhancedInputComponent->BindAction(ForceAction, ETriggerEvent::Triggered, this, &ACannon::Force);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACannon::Fire);
		EnhancedInputComponent->BindAction(FireBoostAction, ETriggerEvent::Started, this, &ACannon::StartCharging);
		EnhancedInputComponent->BindAction(FireBoostAction, ETriggerEvent::Completed, this, &ACannon::Fire);
	}
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation() + (MovementInput* MoveSpeed*DeltaTime);
	SetActorLocation(NewLocation);

	// TODO: should set the limit of the rotation angle
	FRotator NewRotation= Muzzle->GetComponentRotation() + RotationInput*RotationSpeed*DeltaTime;
	Muzzle->SetRelativeRotation(NewRotation);
}

void ACannon::Move(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move called %s"), *Value.ToString());
	MovementInput.X = Value.Get<float>();
}

void ACannon::ProjectileDir(const FInputActionValue& Value)
{
	RotationInput.Roll = Value.Get<float>();
}

void ACannon::Fire(const FInputActionValue& Value)
{
	GetWorld()->GetTimerManager().ClearTimer(SpeedIncreaseTimerHandle);

	
	if (ProjectileEqBasedFactory)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;		// 인스턴스 아닌 클래스로 되는거 같아서 확인해보기
		
		ProjectileEqBased = GetWorld()->SpawnActor<AProjectileEqBased>(ProjectileEqBasedFactory,
			SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), SpawnParams);
	}
	if (FireBoostWidget != nullptr)
	{
		FireBoostWidget->Velocity = 0.0f;	// set velocity var in widget 0
	}
}

void ACannon::StartCharging(const FInputActionValue& Value)
{
	ProjectileVelocity = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(SpeedIncreaseTimerHandle, this, &ACannon::ContinueCharging, 0.1f, true);
	if (FireBoostWidget != nullptr)
	{
		FireBoostWidget->Velocity = ProjectileVelocity;
	}
}

void ACannon::ContinueCharging()
{
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
