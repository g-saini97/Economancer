// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTT_FindCover.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AI/AIController/EnemyAIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AI/CoverSystem/CoverPoint.h"

UBTT_FindCover::UBTT_FindCover()
{
	NodeName = "Find Cover";
}

EBTNodeResult::Type UBTT_FindCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(AIController);

    if (!EnemyAIController)
    {
        return EBTNodeResult::Failed;
    }

    ACoverPoint* CoverPoint = EnemyAIController->FindNearestCoverPoint();
    if (!CoverPoint)
    {
        // Handle no cover found, e.g., set a blackboard key or perform an alternative action
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsVector("CoverLocation", CoverPoint->GetActorLocation());
        //BlackboardComp->SetValueAsObject("CoverPoint", CoverPoint);
    }

    return EBTNodeResult::Succeeded;
}



