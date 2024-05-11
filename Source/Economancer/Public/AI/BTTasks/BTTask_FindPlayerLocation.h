// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Interfaces/BTInterfaces/Aware.h"
#include "BTTask_FindPlayerLocation.generated.h"


/**
 * 
 */
UCLASS()
class ECONOMANCER_API UBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer);

	// override here.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;
	

private:
	// tweak these from the edtior, controll the hard omnidirectional detection range of the NPCs
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	bool SearchRandom = false;// if true, the NPC will randomly look for a place inside the search radius of which, the player is the center.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 135.f; // this is not a radius with the NPC at the center

	//// these bools will be taken in by the NPCS subject to this task. I do not want every NPC in the level to be patroling , many will be stationary untill the player finds them
	bool bIsSubjectAGuard;
	bool bIsSubjectAPatroler;

};
