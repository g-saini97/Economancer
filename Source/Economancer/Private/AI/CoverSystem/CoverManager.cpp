// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

ACoverManager::ACoverManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ACoverManager::BeginPlay()
{
    Super::BeginPlay();

    // Find all cover points in the level
    TArray<AActor*> FoundCoverPoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), FoundCoverPoints);

    for (AActor* Actor : FoundCoverPoints)
    {
        ACoverPoint* CoverPoint = Cast<ACoverPoint>(Actor);
        if (CoverPoint)
        {
            CoverPoints.Add(CoverPoint);
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Found Cover Point: %s"), *CoverPoint->GetName()));
            }
        }
    }
}

void ACoverManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

ACoverPoint* ACoverManager::FindCoverPoint(AActor* NPC, AActor* Player)
{
    // Sort cover points by distance to the NPC
    CoverPoints.Sort([NPC](const ACoverPoint& A, const ACoverPoint& B)
        {
            float DistA = FVector::Dist(A.GetActorLocation(), NPC->GetActorLocation());
            float DistB = FVector::Dist(B.GetActorLocation(), NPC->GetActorLocation());
            return DistA < DistB;
        });

    // Debug output to verify sorting
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Sorted Cover Points:"));
        for (const ACoverPoint* CoverPoint : CoverPoints)
        {
            float Distance = FVector::Dist(CoverPoint->GetActorLocation(), NPC->GetActorLocation());
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%s: %f"), *CoverPoint->GetName(), Distance));
        }
    }

    for (ACoverPoint* CoverPoint : CoverPoints)
    {
        if (!CoverPoint->bIsOccupied && IsCoverPointValid(NPC, Player, CoverPoint))
        {
            CoverPoint->SetOccupied(true);
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Cover Point Selected: %s"), *CoverPoint->GetName()));
            }
            return CoverPoint;
        }
        else
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Cover Point Skipped: %s, Occupied: %s"), *CoverPoint->GetName(), CoverPoint->bIsOccupied ? TEXT("True") : TEXT("False")));
            }
        }
    }
    return nullptr;
}

void ACoverManager::ReleaseCoverPoint(ACoverPoint* CoverPoint)
{
    if (CoverPoint)
    {
        CoverPoint->SetOccupied(false);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Cover Point Released: %s"), *CoverPoint->GetName()));
        }
    }
}

bool ACoverManager::IsCoverPointValid(AActor* NPC, AActor* Player, ACoverPoint* CoverPoint)
{
    FVector PlayerPosition = Player->GetActorLocation();
    FVector CoverPointPosition = CoverPoint->GetActorLocation();

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(CoverPoint);
    CollisionParams.AddIgnoredActor(NPC);

    // Check if cover point has line of sight to the player
    bool bCoverPointCanSeePlayer = GetWorld()->LineTraceSingleByChannel(HitResult, CoverPointPosition, PlayerPosition, ECC_Visibility, CollisionParams);

    // Visualize the line trace
    DrawDebugLine(GetWorld(), CoverPointPosition, PlayerPosition, FColor::Red, false, 1.0f);

    // Check what the trace hit
    if (bCoverPointCanSeePlayer)
    {
        if (HitResult.GetActor() == Player)
        {
            // Trace hit the player, so the player can see the cover point
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Cover point invalid: Player can see it."));
            }
            return false;
        }
        else
        {
            // Trace hit something else, so there's an obstacle
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Cover point valid: Obstacle between cover and player."));
            }
            return true;
        }
    }
    else
    {
        // Trace did not hit anything, meaning the player cannot see the cover point
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Cover point valid: Player cannot see it."));
        }
        return true;
    }
}
