// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STTasks/StateTreeTask_TurnTowardsPlayer.h"
#include "AIController.h"
#include "AI/NPC_AIController.h"
#include "Characters/PlayerCharacter.h"
#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"


EStateTreeRunStatus UStateTreeTask_TurnTowardsPlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
    {
        if (ANPC_AIController* NPCController = Cast<ANPC_AIController>(AIController))
        {
            if (NPCController->HasReachedCover())
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Entering Turn Towards Player State"));
                return EStateTreeRunStatus::Running;
            }
        }
    }
    return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus UStateTreeTask_TurnTowardsPlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    if (AAIController* AIController = Cast<AAIController>(Context.GetOwner()))
    {
        if (ANPC_AIController* NPCController = Cast<ANPC_AIController>(AIController))
        {
            APawn* ControlledPawn = NPCController->GetPawn();
            AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

            if (ControlledPawn && Player)
            {
                FVector Direction = Player->GetActorLocation() - ControlledPawn->GetActorLocation();
                Direction.Z = 0; // Ignore vertical difference
                Direction.Normalize();

                FRotator CurrentRotation = ControlledPawn->GetActorRotation();
                FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
                FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);

                ControlledPawn->SetActorRotation(NewRotation);

                // Check if the NPC is facing the player
                if (FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw)) < 5.0f)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Facing Player"));
                    return EStateTreeRunStatus::Succeeded;
                }
                return EStateTreeRunStatus::Running;
            }
        }
    }
    return EStateTreeRunStatus::Failed;
}

void UStateTreeTask_TurnTowardsPlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
  
    RunStatus = EStateTreeRunStatus::Succeeded;
        EStateTreeRunStatus::Succeeded;
     
}
