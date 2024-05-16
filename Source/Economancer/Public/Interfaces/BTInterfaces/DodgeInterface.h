// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DodgeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDodgeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECONOMANCER_API IDodgeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int DodgeBullet(); // not virtual or = 0 becase this si blueprint native
};
