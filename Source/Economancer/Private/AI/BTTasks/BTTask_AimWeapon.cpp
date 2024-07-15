// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_AimWeapon.h"
#include "AI/NPCs/EnemyNPC.h"
#include "AIController.h"
#include "AI/AIController/EnemyAIController.h"

UBTTask_AimWeapon::UBTTask_AimWeapon()
{
	NodeName = "Aim Weapon";
}

EBTNodeResult::Type UBTTask_AimWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    enemyNPC->SetAimBoolTrue();

    return EBTNodeResult::Succeeded;
}
