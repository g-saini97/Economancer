// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ShotInterFace.h"
#include "Enemy.generated.h"

UCLASS()
class ECONOMANCER_API AEnemy : public ACharacter, public IShotInterFace
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetShot(const FHitResult& hitPoint);

protected:
	virtual void BeginPlay() override;

	float Health = 100;

public:	
	

};
