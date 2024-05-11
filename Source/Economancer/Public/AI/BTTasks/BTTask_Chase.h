// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Chase.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UBTTask_Chase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_Chase(FObjectInitializer const& ObjectInitializer);

	// override here.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;
};
