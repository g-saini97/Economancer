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
#include "Kismet/KismetMathLibrary.h"

AEnemyAIController::AEnemyAIController()
{
    SetupPerceptionSystem();
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!IsValid(Player))
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null in BeginPlay"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter successfully initialized in BeginPlay"));
    }
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
    if (APlayerCharacter* SeenCharacter = Cast<APlayerCharacter>(Actor))
    {
        bCanSeePlayer = Stimulus.WasSuccessfullySensed();
        bHasSeenPlayerOnce = true;
        GetBlackboardComponent()->SetValueAsBool(FName("HasSeenPlayerOnce"), bHasSeenPlayerOnce);
        GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", bCanSeePlayer);
        TArray<AActor*> CoverPoints;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), CoverPoints);
        for (AActor* CoverPoint : CoverPoints)
        {
            if (ACoverPoint* CP = Cast<ACoverPoint>(CoverPoint))
            {
                CP->UpdatePlayerSightStatus(SeenCharacter);
            }
        }
    }
}


// Note to Self, Do Not Disturb this
void AEnemyAIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
        SightConfig->SightRadius = 1800.f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 5.f;
        SightConfig->PeripheralVisionAngleDegrees = 130.f;
        SightConfig->SetMaxAge(5.f);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 1.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
        GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
        GetPerceptionComponent()->ConfigureSense(*SightConfig);
    }
}

// Note to self: If the NPCs exibit janky cover behaviour, start with the behaviour tree first not here 
ACoverPoint* AEnemyAIController::FindNearestCoverPoint()
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return nullptr;

    FVector PawnLocation = ControlledPawn->GetActorLocation();
    TArray<AActor*> CoverPoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverPoint::StaticClass(), CoverPoints);

    ACoverPoint* BestCoverPoint = nullptr;
    float NearestDistance = FLT_MAX;

    for (AActor* CoverPoint : CoverPoints)
    {
        ACoverPoint* CP = Cast<ACoverPoint>(CoverPoint);
        if (CP && !CP->bIsOccupied && !CP->IsInPlayerSight())
        {
            float Distance = FVector::Dist(PawnLocation, CoverPoint->GetActorLocation());
            if (Distance < NearestDistance)
            {
                NearestDistance = Distance;
                BestCoverPoint = CP;
            }
        }
    }
    if (BestCoverPoint)
    {
        VacateCoverPoint();
        OccupyCoverPoint(BestCoverPoint);
    }
    
   
    
    return BestCoverPoint;
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



void AEnemyAIController::UpdateTargetLocation()
{
   
    if (bCanSeePlayer)
    {
        FVector PlayerLocation = Player->GetActorLocation();
        CalculatedTargetLocation = PlayerLocation;
        LastKnownTargetLocation = PlayerLocation;

        GetBlackboardComponent()->SetValueAsVector("TargetLocation", PlayerLocation);
        
    }
    else
    {
        GetBlackboardComponent()->SetValueAsVector("TargetLocation", LastKnownTargetLocation);
    }
}




