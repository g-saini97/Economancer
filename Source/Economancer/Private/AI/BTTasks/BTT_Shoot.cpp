// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTT_Shoot.h"
#include "AI/NPCs/EnemyNPC.h"
#include "AIController.h"
#include "AI/AIController/EnemyAIController.h"


UBTT_Shoot::UBTT_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTT_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AEnemyAIController* enemyAIController = Cast<AEnemyAIController>(AIController);
    if (!enemyAIController)
    {
        return EBTNodeResult::Failed;
    }

    AEnemyNPC* enemyNPC = Cast<AEnemyNPC>(AIController->GetPawn());
    if (!enemyNPC)
    {
        return EBTNodeResult::Failed;
    }

    enemyNPC->Shoot();

    return EBTNodeResult::Succeeded;
}
