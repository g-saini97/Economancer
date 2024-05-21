// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverManager.generated.h"

class ACoverPoint;

UCLASS()
class ECONOMANCER_API ACoverManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoverManager();

	virtual void Tick(float DeltaTime) override;

	static ACoverManager* GetInstance();

	UFUNCTION(BlueprintCallable, Category = "Cover")
	ACoverPoint* FindCoverPoint(AActor* NPC, AActor* Player);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void ReleaseCoverPoint(ACoverPoint* CoverPoint);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	TArray<ACoverPoint*> FindFlankingCoverPoints(AActor* Player, AActor* NPC, float FlankingAngle, float MaxDistance);


protected:
	virtual void BeginPlay() override;

	bool IsCoverPointValid(AActor* NPC, AActor* Player, ACoverPoint* CoverPoint);

	static ACoverManager* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover", meta = (AllowPrivateAccess = "true"))	
	TArray<ACoverPoint*> CoverPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover", meta = (AllowPrivateAccess = "true"))
	float InvalidationRadius = 800.0f; // a Radius around the player that makes cover points invalid so that the NPCs  actually find cover away from the player and not close to them.
};
