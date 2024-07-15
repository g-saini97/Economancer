// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BarryBHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class ECONOMANCER_API ABarryBHUD : public AHUD
{
	GENERATED_BODY()

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }

protected:
	virtual void PreInitializeComponents() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Player HUD")
	TSubclassOf<UPlayerOverlay> PlayerOverLayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
