// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"



void UPlayerOverlay::SetHealthBarPercentage(float Percentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percentage);
	}
}

void UPlayerOverlay::SetEnergyBarPercentage(float Percentage)
{
	if (EnergyBar)
	{
		EnergyBar->SetPercent(Percentage);
	}
}

void UPlayerOverlay::SetTotalRounds(float TotalRoundsInInv)
{
	if (TotalRounds)
	{
		TotalRounds->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), TotalRoundsInInv)));
	}
}

void UPlayerOverlay::SetRoundsInMag(float RoundsInMag)
{
	if (RoundsInMagazine)
	{
		RoundsInMagazine->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), RoundsInMag)));
	}
}
