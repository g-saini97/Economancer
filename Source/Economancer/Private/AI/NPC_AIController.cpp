#include "AI/NPC_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Navigation/PathFollowingComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "AI/NPCs/NPCCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "StateTree.h"
#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"



ANPC_AIController::ANPC_AIController()
{
    StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
    SetupPerceptionSystem();
}


void ANPC_AIController::BeginPlay()
{
    Super::BeginPlay();

    if (StateTreeComponent)
    {
        StateTreeComponent->StartLogic(); // Start the State Tree logic
    }
}

void ANPC_AIController::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);

    if (bCanSeePlayer && GetMoveStatus() != EPathFollowingStatus::Moving)
    {
        TurnTowardsPlayer(DeltaTime);
    }

}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (StateTreeComponent)
    {
        StateTreeComponent->StartLogic(); // Start the State Tree logic
    }

    // Find CoverManager in the world
    TArray<AActor*> FoundCoverManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoverManager::StaticClass(), FoundCoverManagers);

    if (FoundCoverManagers.Num() > 0)
    {
        CoverManager = Cast<ACoverManager>(FoundCoverManagers[0]);
        if (CoverManager)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("CoverManager found"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CoverManager cast failed"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No CoverManager found"));
    }
}


void ANPC_AIController::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    if (SightConfig)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"))); // This sets the current perception component for the system
        SightConfig->SightRadius = 2000.f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;
        SightConfig->PeripheralVisionAngleDegrees = 175.f;
        SightConfig->SetMaxAge(1.f);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 250.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
        GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnTargetDetected);
        GetPerceptionComponent()->ConfigureSense(*SightConfig);
    }
}

void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
    if (auto* const character = Cast <APlayerCharacter>(Actor))
    {
        bCanSeePlayer = Stimulus.WasSuccessfullySensed();
    }
}

void ANPC_AIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    bool bPlayerSeen = false;

    for (AActor* Actor : UpdatedActors)
    {
        if (auto* const character = Cast<APlayerCharacter>(Actor))
        {
            bPlayerSeen = true;
            break;
        }
    }

    bCanSeePlayer = bPlayerSeen;
}

void ANPC_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (Result.IsSuccess())
    {
        bHasReachedCover = true;
        StartCoverTimer(); // Starts the cover timer when the NPC reaches cover
    }
}


FVector ANPC_AIController::FindAndReserveCoverPoint(AActor* Player)
{
    if (CoverManager)
    {
        if (CurrentCoverPoint)
        {
            CoverManager->ReleaseCoverPoint(CurrentCoverPoint);
            CurrentCoverPoint = nullptr;
        }

        ACoverPoint* CoverPoint = CoverManager->FindCoverPoint(GetPawn(), Player);
        if (CoverPoint)
        {
            CurrentCoverPoint = CoverPoint;
            return CoverPoint->GetActorLocation();
        }
    }
    return FVector::ZeroVector;
}

FVector ANPC_AIController::FindAndReserveFlankingCoverPoint(AActor* Player)
{

    if (CoverManager)
    {
        if (CurrentCoverPoint)
        {
            CoverManager->ReleaseCoverPoint(CurrentCoverPoint);
            CurrentCoverPoint = nullptr;
        }

        TArray<ACoverPoint*> FlankingCoverPoints = CoverManager->FindFlankingCoverPoints(Player, GetPawn(), 60.0f, 1000.0f); // Example values for flanking angle and max distance
        if (FlankingCoverPoints.Num() > 0)
        {
            CurrentCoverPoint = FlankingCoverPoints[0]; // Take the nearest flanking cover point
            CurrentCoverPoint->SetOccupied(true);
            return CurrentCoverPoint->GetActorLocation();
        }
    }
    return FVector::ZeroVector;
}

bool ANPC_AIController::TurnTowardsPlayer(float DeltaTime)
{
    APawn* ControlledPawn = this->GetPawn();
    AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (ControlledPawn && Player)
    {
        FVector Direction = Player->GetActorLocation() - ControlledPawn->GetActorLocation();
        Direction.Z = 0; // Ignore vertical difference
        Direction.Normalize();

        FRotator CurrentRotation = ControlledPawn->GetActorRotation();
        FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);
        ControlledPawn->SetActorRotation(NewRotation);

        if (FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw)) < 5.0f)
        {
            return true;
        }
    }

    return false;
}

void ANPC_AIController::ReleaseCurrentCoverPoint()
{
    if (CoverManager && CurrentCoverPoint)
    {
        CoverManager->ReleaseCoverPoint(CurrentCoverPoint);
        CurrentCoverPoint = nullptr;
    }
}

void ANPC_AIController::SetPlayerReference(AActor* Player)
{
    ////// Holdover from when i was using a behavior tree, might use it again.
    //if (StateTreeComponent)
    //{
     //   StateTreeComponent->GetInstanceData()->SetValue("PlayerActor", Player);
    //}
}

void ANPC_AIController::FindCover() /// Called by the STTask, very redundant , will make the STTask just call FindAndReserveCoverPoint() later
{

    if (bCanSeePlayer)
    {

        AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (Player)
        {

            FVector CoverLocation = FindAndReserveCoverPoint(Player);
            if (CoverLocation != FVector::ZeroVector)
            {
                // Move to the cover location
                MoveToCover();
            }
           
        }
       
    }
  
}

void ANPC_AIController::FindFlankingCover()
{
    if (bCanSeePlayer)
    {
        AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (Player)
        {
            FVector CoverLocation = FindAndReserveFlankingCoverPoint(Player);
            if (CoverLocation != FVector::ZeroVector)
            {
                // Move to the cover location
                bIsFlanking = true;
                MoveToCover();
            }
        }
    }
}


void ANPC_AIController::MoveToCover()
{
    if (CurrentCoverPoint)
    {
        MoveToLocation(CurrentCoverPoint->GetActorLocation());
    }
   
}

void ANPC_AIController::StartAiming()
{
    if (ANPCCharacter* NPC = Cast<ANPCCharacter>(this->GetPawn()))
    {
        NPC->SetAimBoolTrue();
    }
}


void ANPC_AIController::StopAiming()
{
    if (ANPCCharacter* NPC = Cast<ANPCCharacter>(GetPawn()))
    {

        NPC->SetAimBoolFalse();
    }
}

void ANPC_AIController::StartCoverTimer()
{
    bCoverTimerExpired = false;
    GetWorld()->GetTimerManager().SetTimer(CoverTimerHandle, [this](){bCoverTimerExpired = true;}, 60.0f, false); /// tweak this timer if the NPCs are not staying in cover long enough
}




