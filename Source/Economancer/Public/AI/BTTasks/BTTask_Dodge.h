// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/NPCs/NPCCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class ECONOMANCER_API UBTTask_Dodge : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Dodge();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	bool MontageHasFinished(ANPCCharacter* const NPC);

};
