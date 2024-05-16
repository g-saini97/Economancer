// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_FindCoverPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NPC_AIController.h"

UBTTask_FindCoverPoint::UBTTask_FindCoverPoint()
{
	NodeName = "Find Cover Point";
}

EBTNodeResult::Type UBTTask_FindCoverPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ANPC_AIController* AIController = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerActor"));
    if (!Player)
    {
        return EBTNodeResult::Failed;
    }

    FVector CoverLocation = AIController->FindAndReserveCoverPoint(Player);
    if (CoverLocation != FVector::ZeroVector)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), CoverLocation);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}


