// Fill out your copyright notice in the Description page of Project Settings.

#include "Stocks/baseStock.h"

#include "Economancer/DebugMacros.h"

// Sets default values
AbaseStock::AbaseStock()
{
	PrimaryActorTick.bCanEverTick = true;
	// assigning values to class vars
	amp = 5.f;
	freq = 10.f;
	moveRate = 50.f;
	rotationRate = 6.f;
}

void AbaseStock::BeginPlay()
{
	Super::BeginPlay();

	// Setting default Location and rotation
	SetActorLocation(FVector(0.f, 0.f, 20.f));
	SetActorRotation(FRotator(0.f, 45.f, 0.f));

	// Getting the world
	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	FVector forwardVec = GetActorForwardVector();


	// Debug Gizmos
	//DEBUG_SPHERE_SPAWNER(Location);
	//DEBUG_DRAW_FORWARD_VECTOR(Location, Location + forwardVec * 50.f);
	
}

float AbaseStock::transformSinFunc(float value)
{
	return  FMath::Sin(value * amp);
}

// Called every frame
void AbaseStock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Location = GetActorLocation();
	FVector forwardVec = GetActorForwardVector();
	
	runningTime += DeltaTime;
	AddActorWorldOffset(FVector(moveRate * DeltaTime, 0.f, transformSinFunc(runningTime)));
	
	
	DEBUG_SPHERE_SPAWNER_PER_FRAME(Location);
	DEBUG_DRAW_FORWARD_VECTOR_PER_FRAME(Location, Location + forwardVec * 50);
}

