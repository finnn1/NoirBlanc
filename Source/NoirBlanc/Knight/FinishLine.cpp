// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishLine.h"

#include "Components/BoxComponent.h"

// Sets default values
AFinishLine::AFinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	//BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	//SetRootComponent(BoxComp);
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFinishLine::OnOverlap);
}

// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishLine::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("fdxgsdgdsg"));
}

