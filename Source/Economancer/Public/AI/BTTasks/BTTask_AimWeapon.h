// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AimWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UBTTask_AimWeapon : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_AimWeapon();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
