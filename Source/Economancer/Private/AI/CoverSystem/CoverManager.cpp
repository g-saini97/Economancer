// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"



ACoverManager* ACoverManager::Instance = nullptr;

ACoverManager::ACoverManager()
{
    PrimaryActorTick.bCanEverTick = true;
    Instance = this;
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
        }
    }
}

ACoverManager* ACoverManager::GetInstance()
{
    return Instance;
}

void ACoverManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

ACoverPoint* ACoverManager::FindCoverPoint(AActor* NPC, AActor* Player)
{
    CoverPoints.Sort([NPC](const ACoverPoint& A, const ACoverPoint& B)
        {
            float DistA = FVector::Dist(A.GetActorLocation(), NPC->GetActorLocation());
            float DistB = FVector::Dist(B.GetActorLocation(), NPC->GetActorLocation());
            return DistA < DistB;
        });

    for (ACoverPoint* CoverPoint : CoverPoints)
    {
        if (!CoverPoint->bIsOccupied && IsCoverPointValid(NPC, Player, CoverPoint))
        {
            CoverPoint->SetOccupied(true);
           //if (GEngine)
           // {
           //     GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Cover Point Selected: %s"), *CoverPoint->GetName()));
           // }
            return CoverPoint;
        }
    }
    return nullptr;
}

TArray<ACoverPoint*> ACoverManager::FindFlankingCoverPoints(AActor* Player, AActor* NPC, float FlankingAngle, float MaxDistance)
{
    TArray<ACoverPoint*> FlankingCoverPoints;
    FVector PlayerLocation = Player->GetActorLocation();
    FVector PlayerForward = Player->GetActorForwardVector();

    for (ACoverPoint* CoverPoint : CoverPoints)
    {
        if (!CoverPoint->bIsOccupied && IsCoverPointValid(NPC, Player, CoverPoint))
        {
            FVector ToCoverPoint = CoverPoint->GetActorLocation() - PlayerLocation;
            float Distance = ToCoverPoint.Size();
            ToCoverPoint.Normalize();

            float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerForward, ToCoverPoint)));
            if (Angle <= FlankingAngle && Distance <= MaxDistance)
            {
                // Check if the cover point is visible to the player
                FHitResult HitResult;
                FCollisionQueryParams CollisionParams;
                CollisionParams.AddIgnoredActor(CoverPoint);
                CollisionParams.AddIgnoredActor(NPC);

                bool bPlayerCanSeeCover = GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLocation, CoverPoint->GetActorLocation(), ECC_Visibility, CollisionParams);

                // Visualize the line trace
                DrawDebugLine(GetWorld(), PlayerLocation, CoverPoint->GetActorLocation(), FColor::Red, false, 1.0f);

                // If the player cannot see the cover point, add it to the list
                if (!bPlayerCanSeeCover || (HitResult.GetActor() && HitResult.GetActor() != Player))
                {
                    FlankingCoverPoints.Add(CoverPoint);
                }
            }
        }
    }

    // Sort flanking cover points by distance to the NPC
    FlankingCoverPoints.Sort([NPC](const ACoverPoint& A, const ACoverPoint& B)
        {
            float DistA = FVector::Dist(A.GetActorLocation(), NPC->GetActorLocation());
            float DistB = FVector::Dist(B.GetActorLocation(), NPC->GetActorLocation());
            return DistA < DistB;
        });

    return FlankingCoverPoints;
}

void ACoverManager::ReleaseCoverPoint(ACoverPoint* CoverPoint)
{
    if (CoverPoint)
    {
        CoverPoint->SetOccupied(false);
        //if (GEngine)
        //{
        //    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Cover Point Released: %s"), *CoverPoint->GetName()));
        //}
    }
}



bool ACoverManager::IsCoverPointValid(AActor* NPC, AActor* Player, ACoverPoint* CoverPoint)
{
    FVector PlayerPosition = Player->GetActorLocation();
    FVector CoverPointPosition = CoverPoint->GetActorLocation();

    // Check if the cover point is within the invalidation radius of the player
    float DistanceToPlayer = FVector::Dist(PlayerPosition, CoverPointPosition);
    if (DistanceToPlayer <= InvalidationRadius)
    {
        //if (GEngine)
        //{
        //    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Cover point invalid: Too close to player."));
        //}
        return false;
    }


    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(CoverPoint);
    CollisionParams.AddIgnoredActor(NPC);

    bool bCoverPointCanSeePlayer = GetWorld()->LineTraceSingleByChannel(HitResult, CoverPointPosition, PlayerPosition, ECC_Visibility, CollisionParams);

    DrawDebugLine(GetWorld(), CoverPointPosition, PlayerPosition, FColor::Red, false, 1.0f);

    if (bCoverPointCanSeePlayer)
    {
        if (HitResult.GetActor() == Player)
        {
            //if (GEngine)
            //{
            //    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("Cover point invalid: Player can see it."));
            //}
            return false;
        }
        else
        {
            //if (GEngine)
            //{
            //    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Cover point valid: Obstacle between cover and player."));
            //}
            return true;
        }
    }
    else
    {
        //if (GEngine)
        //{
        //    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Cover point valid: Player cannot see it."));
        //}
        return true;
    }
}