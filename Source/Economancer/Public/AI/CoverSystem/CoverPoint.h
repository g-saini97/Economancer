// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverPoint.generated.h"

class AEnemyAIController;
UCLASS()
class ECONOMANCER_API ACoverPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	FORCEINLINE bool GetCoverStatus() const { return bIsOccupied; };


	ACoverPoint();

	virtual void Tick(float DeltaTime) override;

	void Occupy(AEnemyAIController* NewOccupant);
	void Vacate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover System")
	bool bIsOccupied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover System")
	AEnemyAIController* Occupant;

protected:
	virtual void BeginPlay() override;

};
