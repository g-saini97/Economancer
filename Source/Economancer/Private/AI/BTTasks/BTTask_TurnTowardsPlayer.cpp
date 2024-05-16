// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_TurnTowardsPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AI/NPC_AIController.h"

UBTTask_TurnTowardsPlayer::UBTTask_TurnTowardsPlayer()
{
	NodeName = "Turn Towards Player";
}

EBTNodeResult::Type UBTTask_TurnTowardsPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    FVector LastKnownPlayerLocation = BlackboardComp->GetValueAsVector("LastKnownPlayerLocation");

    // Get direction to look at
    FVector Direction = LastKnownPlayerLocation - ControlledPawn->GetActorLocation();
    Direction.Z = 0; // Ignore vertical difference
    Direction.Normalize();

    // Calculate the new rotation
    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();

    // Set the new rotation
    ControlledPawn->SetActorRotation(NewLookAt);

    return EBTNodeResult::Succeeded;
}
