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

    if (auto const NPCActor = Cast<ANPCCharacter>(NPCController->GetPawn()))
    {
     
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