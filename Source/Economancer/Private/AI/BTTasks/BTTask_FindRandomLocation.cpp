// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NPC_AIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	// get the AI controller and its owner(NPC)..../// Learned about the auto keyword here, wow
	if (auto* const controller = Cast<ANPC_AIController>(OwnerComponent.GetAIOwner()))
	{
		if (auto* const NPC = controller->GetPawn()) // get the Enemy NPC
		{
			auto const Origin = NPC->GetActorLocation(); // get the location of the Enemy NPC
			
			if (auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld())) // get the navigation system of the current world 
			{
				
				FNavLocation Loc;
				if (NavigationSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc)) // looks for a random point on the navmesh and puts the output in Location
				{
					OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location); // setting the key in the related blackboard [Check out the blackboard BB_Enemy in the editor for context]
				}
				// if task succedes 
				FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed; // if task fails
}
