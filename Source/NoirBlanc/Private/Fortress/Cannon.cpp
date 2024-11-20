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
 }

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
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
	if (ProjectileFactory)
	{
		FRotator ProjectileRotation = Muzzle->GetComponentRotation();
		FVector RotatedVector = ProjectileRotation.Vector();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileFactory,
			SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), SpawnParams);
		
		
		//	UE_LOG(LogTemp, Warning, TEXT("%s"), *ProjectileRotation.ToString());
		
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Projectile->LaunchDirection.ToString());
		// Projectile->Owner = this;
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *Projectile->GetOwner()->GetName());
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString::Printf(TEXT("%s"), *Muzzle->GetComponentLocation().ToString()));
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *Projectile->GetActorLocation().ToString()));

	}
	// CreateDefaultSubobject is for generating component
}

void ACannon::Force(const FInputActionValue& Value)
{
}
