// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeTask_FindFlankingCover.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UStateTreeTask_FindFlankingCover : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
public:

    // Function called when entering the state
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

    // Function called each frame the state is active
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

    // Function called when exiting the state
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float AcceptanceRadius = 100.0f;  // How close the NPC needs to get to the cover point

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float FlankingAngle = 90.0f;  // Angle for flanking cover

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float MaxFlankingDistance = 1000.0f;  // Max distance for flanking cover
};
