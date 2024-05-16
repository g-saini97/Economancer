// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "NPC_AIController.generated.h"


class UAISenseConfig_Sight;
class ACoverManager;
class ACoverPoint;
/**
 * 
 */
UCLASS()
class ECONOMANCER_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANPC_AIController();


	// cover manager related functions and variables 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	ACoverManager* CoverManager;

	FVector FindAndReserveCoverPoint(AActor* Player); /// BTTask that decides if the npc needs to find cover has to be able to access this.
	void SetPlayerReference(AActor* Player); // Sets the PlayerAcor key int e blackBoard.
	void ReleaseCurrentCoverPoint();

protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:

	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetdetected(AActor* Actor, FAIStimulus const Stimulus);

	ACoverPoint* CurrentCoverPoint;
};
