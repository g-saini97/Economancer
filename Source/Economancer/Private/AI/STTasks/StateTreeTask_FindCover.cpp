

#include "AI/STTasks/StateTreeTask_FindCover.h"
#include "AIController.h"
#include "AI/NPC_AIController.h"
#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus UStateTreeTask_FindCover::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
    {
        if (ANPC_AIController* NPCController = Cast<ANPC_AIController>(AIController))
        {
            NPCController->FindCover();
            RunStatus = EStateTreeRunStatus::Running;
            return EStateTreeRunStatus::Running;
        }
    }
    FinishTask(false);
    RunStatus = EStateTreeRunStatus::Failed;
    return EStateTreeRunStatus::Failed;

}

EStateTreeRunStatus UStateTreeTask_FindCover::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
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
                    if (!bIsInCover && NPCController->HasCoverTimeExpired())
                    {
                        // NPC has reached cover for the first time
                        bIsInCover = true;
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Cover Reached"));
                    }

                    // Wait for the cover timer to expire
                    else
                    {
                        return EStateTreeRunStatus::Failed;
                    }
                }
            }
        }
    }
    RunStatus = EStateTreeRunStatus::Succeeded;
    return EStateTreeRunStatus::Succeeded;
}

void UStateTreeTask_FindCover::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    FinishTask(true);
    RunStatus = EStateTreeRunStatus::Succeeded;
    EStateTreeRunStatus::Succeeded;
}


