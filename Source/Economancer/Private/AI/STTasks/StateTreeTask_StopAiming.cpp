// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STTasks/StateTreeTask_StopAiming.h"
#include "AIController.h"
#include "AI/NPC_AIController.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus UStateTreeTask_StopAiming::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
    {
        if (ANPC_AIController* NPCController = Cast<ANPC_AIController>(AIController))
        {
            NPCController->StopAiming();
            RunStatus = EStateTreeRunStatus::Succeeded;
            return EStateTreeRunStatus::Succeeded;
        }
    }
    RunStatus = EStateTreeRunStatus::Failed;
    return EStateTreeRunStatus::Failed;
}

