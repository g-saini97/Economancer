// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShotInterFace.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShotInterFace : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECONOMANCER_API IShotInterFace
{
	GENERATED_BODY()

public:

	virtual void GetShot(const FHitResult& hit) = 0; // pure virtual
};
