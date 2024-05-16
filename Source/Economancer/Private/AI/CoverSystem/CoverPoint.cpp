// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CoverSystem/CoverPoint.h"
#include "Components/SceneComponent.h"

// Sets default values
ACoverPoint::ACoverPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CoverPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CoverPoint"));
	RootComponent = CoverPoint;

	bIsOccupied = false; // by default a cover point should alwyas be unoccupied, The NPCs need to be able to change this 

}

// Called when the game starts or when spawned
void ACoverPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoverPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACoverPoint::SetOccupied(bool bOccupied)
{
	bIsOccupied = bOccupied; 
}

