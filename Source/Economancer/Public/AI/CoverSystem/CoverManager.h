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


	UFUNCTION(BlueprintCallable, Category = "Cover")
	ACoverPoint* FindCoverPoint(AActor* NPC, AActor* Player);

	UFUNCTION(BlueprintCallable, Category = "Cover")
	void ReleaseCoverPoint(ACoverPoint* CoverPoint);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover")
	TArray<ACoverPoint*> CoverPoints;


protected:
	virtual void BeginPlay() override;

	bool IsCoverPointValid(AActor* NPC, AActor* Player, ACoverPoint* CoverPoint);

};
