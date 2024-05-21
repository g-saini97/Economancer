// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Components/StateTreeComponent.h"
#include "NPC_AIController.generated.h"

class UAISenseConfig_Sight;
//class UStateTreeComponent;
class UStateTree;
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
    
    FORCEINLINE bool HasReachedCover() const { return bHasReachedCover; }
    FORCEINLINE bool HasCoverTimeExpired() const { return bHasReachedCover; }

    // Cover manager related functions and variables 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    ACoverManager* CoverManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    bool bCanSeePlayer = false; // has to be seen by the STTask

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
    float TurnSpeed = 15.f; // this would be degrees per second

 
    ACoverPoint* CurrentCoverPoint; 
    

    UFUNCTION(BlueprintCallable)
    void FindCover();

    UFUNCTION(BlueprintCallable)
    void FindFlankingCover();

    UFUNCTION(BlueprintCallable)
    void MoveToCover();

    UFUNCTION(BlueprintCallable)
    void StartAiming();

    UFUNCTION(BlueprintCallable)
    void StopAiming();

    UFUNCTION(BlueprintCallable)
    void StartCoverTimer();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override; 
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
    TObjectPtr<UAISenseConfig_Sight> SightConfig;
    FTimerHandle CoverTimerHandle; // For the Time that NPCs wait to change cover positions. If they are changing cover too fast and are not stayin in place, check this timer

    void SetupPerceptionSystem();

    bool TurnTowardsPlayer(float DeltaTime);
    FVector FindAndReserveCoverPoint(AActor* Player);
    FVector  FindAndReserveFlankingCoverPoint(AActor* Player);
    void SetPlayerReference(AActor* Player);
    void ReleaseCurrentCoverPoint();


    UFUNCTION()
    void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

    // to detect when the npc looses sight of the player, the State tree needs to see this for proprer state transitions
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    bool bHasReachedCover = false;
    bool bIsFlanking = false;
    bool bCoverTimerExpired = false;

    // StateTree Component
    UPROPERTY(EditAnywhere, Category = "AI")
    UStateTreeComponent* StateTreeComponent;

    UPROPERTY(EditAnywhere, Category = "AI")
    UStateTree* StateTreeAsset;
};
