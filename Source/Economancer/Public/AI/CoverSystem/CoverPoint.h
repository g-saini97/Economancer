// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverPoint.generated.h"

class USceneComponent;

UCLASS()
class ECONOMANCER_API ACoverPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoverPoint();

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover")
	USceneComponent* CoverPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover")
	bool bIsOccupied;

	void SetOccupied(bool bOccupied);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
