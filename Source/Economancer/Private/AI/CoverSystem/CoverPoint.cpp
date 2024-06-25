// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CoverSystem/CoverPoint.h"

// Sets default values
ACoverPoint::ACoverPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsOccupied = false;
	Occupant = nullptr; 
}

void ACoverPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoverPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoverPoint::Occupy(AEnemyAIController* NewOccupant)
{
	bIsOccupied = true;
	Occupant = NewOccupant;
}

void ACoverPoint::Vacate()
{
	bIsOccupied = false;
	Occupant = nullptr;
}

