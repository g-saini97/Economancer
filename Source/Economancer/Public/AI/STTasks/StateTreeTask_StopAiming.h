// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeTask_StopAiming.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UStateTreeTask_StopAiming : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	// Did not override tick for this because the purpose of this task is to flip a bool and nothing else.

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
};
