// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ECONOMANCER_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetHealthBarPercentage(float Percentage);
	void SetEnergyBarPercentage(float Percentage);
	void SetTotalRounds(float TotalRoundsInInv);
	void SetRoundsInMag(float RoundsInMag);

private:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnergyBar;

	UPROPERTY(meta = (BindWdget))
	UTextBlock* RoundsInMagazine;

	UPROPERTY(meta = (BindWdget))
	UTextBlock* TotalRounds;

	

};
