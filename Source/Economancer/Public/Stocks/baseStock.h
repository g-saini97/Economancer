// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "baseStock.generated.h"



UCLASS()
class ECONOMANCER_API AbaseStock : public AActor
{
	GENERATED_BODY()
	
public:	
	AbaseStock();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stock Parameters")
	float price;

	float transformSinFunc(float value);

private:
	float moveRate;
	float rotationRate;
	float runningTime;
	float amp ;
	float freq ;

	
};
