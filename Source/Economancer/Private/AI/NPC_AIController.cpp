// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC_AIController.h"
#include "Pawns/Enemy.h"


ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInititalizer)
{

}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemy* enemy = Cast<AEnemy>(InPawn))
	{
		if (UBehaviorTree* tree = enemy->GetBehaviorTree())
		{
			UBlackboardComponent* b = nullptr;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);


		}

	}
}