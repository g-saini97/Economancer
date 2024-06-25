// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTT_MoveToCover.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIController/EnemyAIController.h"

UBTT_MoveToCover::UBTT_MoveToCover()
{
    NodeName = "Move To Cover";

}

EBTNodeResult::Type UBTT_MoveToCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    FVector CoverLocation = BlackboardComp->GetValueAsVector("CoverLocation");

    if (CoverLocation.IsZero())
    {
        return EBTNodeResult::Failed;
    }

    AIController->MoveToLocation(CoverLocation, -1.0f, true, true, false, true, 0, true);

    return EBTNodeResult::Succeeded;
}
