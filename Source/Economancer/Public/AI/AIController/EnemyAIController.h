// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class ACoverPoint;
class APlayerCharacter;
class AEnemyNPC;

UCLASS()
class ECONOMANCER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    FORCEINLINE FVector GetCalculatedTargetLocation() const {return CalculatedTargetLocation; };
    FORCEINLINE FVector GetLastKnownTargetLocation() const { return LastKnownTargetLocation; };

    AEnemyAIController();

    //Cover system related components here, will change them to uobjectptr later.
    ACoverPoint* FindNearestCoverPoint();
    void OccupyCoverPoint(ACoverPoint* CoverPoint);
    void VacateCoverPoint();
    void UpdateTargetLocation();

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

    UPROPERTY()
    FVector LastKnownTargetLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    bool bCanSeePlayer = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    bool bHasSeenPlayerOnce = false; // the spooko bool, lest the npc access teh last known location of the player after have seen them once, important for the behavor tree to not mess wtih npc oreintation at spawn.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    FVector CalculatedTargetLocation = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    APlayerCharacter* Player = nullptr; // a ref to the player when the npcs sees em' ;




};
