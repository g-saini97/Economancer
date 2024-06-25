// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class ACoverPoint;

UCLASS()
class ECONOMANCER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    AEnemyAIController();

    //Cover system related components here, will change them to uobjectptr later.
    ACoverPoint* FindNearestCoverPoint();
    void OccupyCoverPoint(ACoverPoint* CoverPoint);
    void VacateCoverPoint();

protected:
    virtual void BeginPlay() override;

    virtual void OnPossess(APawn* InPawn) override;


private:

    UAISenseConfig_Sight* SightConfig;

    void SetupPerceptionSystem();

    UFUNCTION()
    void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

    UPROPERTY()
    ACoverPoint* CurrentCoverPoint;
  
};
