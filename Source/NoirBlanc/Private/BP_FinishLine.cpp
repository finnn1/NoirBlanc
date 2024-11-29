// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_FinishLine.h"

#include "Components/BoxComponent.h"

// Sets default values
ABP_FinishLine::ABP_FinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABP_FinishLine::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ABP_FinishLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_FinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABP_FinishLine::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 
}
