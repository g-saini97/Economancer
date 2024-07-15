#include "AI/BTTasks/BTTask_LookAtLastKnownLocation.h"
#include "AIController.h"
#include "AI/AIController/EnemyAIController.h"
#include "AI/NPCs/EnemyNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"


UBTTask_LookAtLastKnownLocation::UBTTask_LookAtLastKnownLocation()
{
    NodeName = "Look At Last Known Location";
}

EBTNodeResult::Type UBTTask_LookAtLastKnownLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AEnemyAIController* NPCController = Cast<AEnemyAIController>(AIController);
    if (NPCController)
    {
        AEnemyNPC* NPC = Cast<AEnemyNPC>(NPCController->GetPawn());
        if (NPC)
        {
            // Check if NPC is stationary
            if (NPC->GetCharacterMovement()->Velocity.SizeSquared() <= KINDA_SMALL_NUMBER)
            {
                NPCController->UpdateTargetLocation();
                FVector LastKnownLocation = NPCController->GetLastKnownTargetLocation();
                RotateToTarget(NPC, LastKnownLocation);
                return EBTNodeResult::Succeeded;
            }
        }
    }
    return EBTNodeResult::Failed;
}

void UBTTask_LookAtLastKnownLocation::RotateToTarget(AEnemyNPC* NPC, const FVector& TargetLocation)
{
    FVector Direction = (TargetLocation - NPC->GetActorLocation()).GetSafeNormal();
    FRotator LookAtRotation = UKismetMathLibrary::MakeRotFromX(Direction);
    NPC->SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f)); // Only rotate around the yaw axis
}