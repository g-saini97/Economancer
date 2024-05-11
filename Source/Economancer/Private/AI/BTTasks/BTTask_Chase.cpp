// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_Chase.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AI/NPCs/NPCCharacter.h"
#include "AI/NPC_AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UBTTask_Chase::UBTTask_Chase(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{

	if (auto* const controller = Cast<ANPC_AIController>(OwnerComponent.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComponent.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, PlayerLocation);

		FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
