// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController/EnemyAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "AI/NPCs/EnemyNPC.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Actor.h"

AEnemyAIController::AEnemyAIController()
{
    SetupPerceptionSystem();
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (AEnemyNPC* NPC = Cast<AEnemyNPC>(InPawn))
    {
        if (UBehaviorTree* BTree = NPC->GetBehaviorTree())
        {
            UBlackboardComponent* BBComponent;
            UseBlackboard(BTree->BlackboardAsset, BBComponent);
            Blackboard = BBComponent;
            RunBehaviorTree(BTree);
       
        }
    }
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
    if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(Actor))
    {
        GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
    }
}


// Note to Self, Do Not Disturb this
void AEnemyAIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
        SightConfig->SightRadius = 500.f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        SightConfig->SetMaxAge(7.f);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = SightConfig->SightRadius + 5.f; // keeping this small to keep the last known location of the player in a tighter zone
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
        GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected); // To call OnTargetDetected when player or item is sensed , OntargetDetected handles what to do when the player is sensed.
        GetPerceptionComponent()->ConfigureSense(*SightConfig); 
    }
}

// Note to self: If the NPCs exibit janky cover behaviour, start with the behaviour tree first not here 
ACoverPoint* AEnemyAIController::FindNearestCoverPoint()
{
    if (CurrentCoverPoint)
    {
        return CurrentCoverPoint;
    }
    else 
    {
        APawn* ControlledPawn = GetPawn();
        if (!ControlledPawn) return nullptr;

        FVector PawnLocation = ControlledPawn->GetActorLocation();
        TArray<AActor*> CoverPoints;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), CoverPoints);

        ACoverPoint* NearestCoverPoint = nullptr;
        float NearestDistance = FLT_MAX;

        for (AActor* CoverPoint : CoverPoints)
        {
            ACoverPoint* CP = Cast<ACoverPoint>(CoverPoint);
            if (CP && !CP->bIsOccupied)
            {
                float Distance = FVector::Dist(PawnLocation, CoverPoint->GetActorLocation());
                if (Distance < NearestDistance)
                {
                    NearestDistance = Distance;
                    NearestCoverPoint = CP;
                }
            }
        }

        if (NearestCoverPoint)
        {
            OccupyCoverPoint(NearestCoverPoint);
        }
        return NearestCoverPoint;
    }
    
}

void AEnemyAIController::OccupyCoverPoint(ACoverPoint* CoverPoint)
{
    if (CoverPoint)
    {
        CurrentCoverPoint = CoverPoint;
        CoverPoint->Occupy(this);
    }
}

// has to be called by the BTTask for finding cover , should also be called at death (Handle that in the enemy npc's class) .
void AEnemyAIController::VacateCoverPoint()
{
    if (CurrentCoverPoint)
    {
        CurrentCoverPoint->Vacate();
        CurrentCoverPoint = nullptr;
    }
}

