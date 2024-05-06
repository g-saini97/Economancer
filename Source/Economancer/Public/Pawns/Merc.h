// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Merc.generated.h"

//Forward Declarations
class UCapsuleComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;


UCLASS()
class ECONOMANCER_API AMerc : public ACharacter
{
	GENERATED_BODY()

public:
	AMerc();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float hitpoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float hireCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float wagePerMission;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool isHired ;



private:
// useing TobjectPtr instead of doing the usuall "className* ptr" declaration ;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mercMesh;


};
