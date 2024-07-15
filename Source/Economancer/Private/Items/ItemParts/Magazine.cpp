// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemParts/Magazine.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
AMagazine::AMagazine()
{

    PrimaryActorTick.bCanEverTick = true;

    // Initialize the magazine mesh
    MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMesh"));
    RootComponent = MagazineMesh;

    // Enable physics for the magazine mesh
    MagazineMesh->SetSimulatePhysics(true);

    // Initialize the physics handle component (optional)
    PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

}

// Called when the game starts or when spawned
void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

