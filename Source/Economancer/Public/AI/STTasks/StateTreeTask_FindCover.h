// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeTask_FindCover.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UStateTreeTask_FindCover : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
    //UStateTreeTask_FindCover();

    // Function called when entering the state
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

    // Function called each frame the state is active
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

    // Function called when exiting the state
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float AcceptanceRadius = 50.0f;  // How close the NPC needs to get to the cover point
	
    bool bIsInCover = false;
};
