// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Merc.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h" 

// Sets default values
AMerc::AMerc()
{
	PrimaryActorTick.bCanEverTick = true;


	mercMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mercMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMerc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMerc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMerc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

