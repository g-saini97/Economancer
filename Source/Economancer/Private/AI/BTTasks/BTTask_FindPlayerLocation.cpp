// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AI/NPCs/NPCCharacter.h"
#include "AI/NPC_AIController.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Player's Location on NavMesh";
}

/*EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	// get player character
	if (auto* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		// getting the AINPC's controller and pawn's location
		auto* const NPCController = Cast<ANPC_AIController>(OwnerComponent.GetAIOwner());
		auto* const NPCPawn = NPCController->GetPawn();
		auto const NPCLocation = NPCPawn->GetActorLocation();
		// get player location
		auto const PlayerLocation = PlayerCharacter->GetActorLocation();

		if (SearchRandom)
		{
			FNavLocation loc;
			//get the nav system to get a random location around the NPC, dictated by the search radius
			if (auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				if (NavigationSystem->GetRandomPointInNavigableRadius(NPCLocation, SearchRadius, loc)) // loc stores the output of this function
				{
					OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), loc.Location); // setting the keylocation to a random point with the search radius of the player
					FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded); // first success condition
					return EBTNodeResult::Succeeded;
				}

			}
		}
		else

		{
			OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation); // Key set to the exact player location
			FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded); // second success condition
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
*/

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
    auto* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Player characterFound"));
        return EBTNodeResult::Failed;
    }

    auto* const NPCController = Cast<ANPC_AIController>(OwnerComponent.GetAIOwner());
    auto* const NPCPawn = NPCController ? NPCController->GetPawn() : nullptr;
    if (!NPCPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("No NPC PAWN Found"));
        return EBTNodeResult::Failed;
    }

    auto const NPCLocation = NPCPawn->GetActorLocation();
    auto const PlayerLocation = PlayerCharacter->GetActorLocation();

    if (SearchRandom)
    {
        FNavLocation RandomLocation;
        auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavigationSystem)
        {
            if (NavigationSystem->GetRandomPointInNavigableRadius(NPCLocation, SearchRadius, RandomLocation))
            {
                OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation.Location);
                FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
                return EBTNodeResult::Succeeded;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to get random point in navigable radius for NPC"));
        }
    }
    else
    {
        OwnerComponent.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
    }

    FinishLatentTask(OwnerComponent, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}