// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

class UPhysicsHandleComponent;

UCLASS()
class ECONOMANCER_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagazine();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* MagazineMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UPhysicsHandleComponent* PhysicsHandle;

};
