// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STTasks/StateTreeTask_FindFlankingCover.h"
#include "AIController.h"
#include "AI/NPC_AIController.h"
#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"




EStateTreeRunStatus UStateTreeTask_FindFlankingCover::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
    {
        if (ANPC_AIController* NPCController = Cast<ANPC_AIController>(AIController))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Entering Find Flanking Cover State"));
            NPCController->FindFlankingCover();
            return EStateTreeRunStatus::Running;
        }
    }
    FinishTask(false);
    RunStatus = EStateTreeRunStatus::Failed;
    return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus UStateTreeTask_FindFlankingCover::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
    {
        if (ANPC_AIController* NPCController = Cast<ANPC_AIController>(AIController))
        {
            APawn* ControlledPawn = NPCController->GetPawn();
            if (ControlledPawn && NPCController->CurrentCoverPoint)
            {
                float DistanceToCover = FVector::Dist(ControlledPawn->GetActorLocation(), NPCController->CurrentCoverPoint->GetActorLocation());
                if (DistanceToCover <= AcceptanceRadius) // Use the task's AcceptanceRadius
                {
                    // NPC has reached cover
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Cover Reached"));
                    return EStateTreeRunStatus::Succeeded;
                }
            }
        }
    }
    return EStateTreeRunStatus::Running;
}

void UStateTreeTask_FindFlankingCover::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exiting Find Flanking Cover State"));
}
