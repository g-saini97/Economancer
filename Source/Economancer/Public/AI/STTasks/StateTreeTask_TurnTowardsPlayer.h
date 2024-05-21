// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeTask_TurnTowardsPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UStateTreeTask_TurnTowardsPlayer : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:
    //UStateTreeTask_TurnTowardsPlayer();

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
    float TurnSpeed = 15.0f; // this is in degrees per second
};
