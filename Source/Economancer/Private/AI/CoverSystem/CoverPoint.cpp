// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CoverSystem/CoverPoint.h"
#include "Characters/PlayerCharacter.h"

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

bool ACoverPoint::IsInPlayerSight() const
{

    return bIsInPlayerSight;
}

void ACoverPoint::UpdatePlayerSightStatus(AActor* PlayerActor) //This is seperate so that cover points do not have to tick all the time and will only Update when an npc sees the player.
{
    if (!PlayerActor) return;

    FVector Start = PlayerActor->GetActorLocation();
    FVector End = GetActorLocation();
    FHitResult HitResult;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignore the cover point itself

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        bIsInPlayerSight = HitActor && HitActor->IsA(APlayerCharacter::StaticClass());

        // Debuggers
        //FColor LineColor = bIsInPlayerSight ? FColor::Red : FColor::Green;
        //DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f);
        bIsInPlayerSight = true;
    }
    else
    {
        bIsInPlayerSight = false;
    }
}