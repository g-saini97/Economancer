// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "NPC_AIController.generated.h"


class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class ECONOMANCER_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ANPC_AIController(FObjectInitializer const& ObjectInititalizer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:

	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetdetected(AActor* Actor, FAIStimulus const Stimulus);

};
