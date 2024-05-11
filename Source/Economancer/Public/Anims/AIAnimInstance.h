// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI/AICharacterTypes.h"
#include "AIAnimInstance.generated.h"


class ANPCCharacter;
class UCharacterMovementComponent;
class ANPC_AIController;
/**
 * 
 */
UCLASS()
class ECONOMANCER_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	FRotator calculateYawPitch();

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ANPCCharacter> AICharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ANPC_AIController> AIController;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> AIMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	float xySpeed;// character's speed on the ground

	UPROPERTY(BlueprintReadOnly, Category = "Aim Direction");
	float yaw;

	UPROPERTY(BlueprintReadOnly, Category = "Aim Direction");
	float pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	bool isAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	EAIState AIState;

	/////// vars for yaw and pitch calculations or the aim offset. Do not touch (aiming will break)
	FRotator CurrentRotation;
	FRotator interpedRotation;
	FRotator clampedInterpRotation; // want rotation to stay between -90 to 90 degrees;
	FRotator controlerRotaion;
	FRotator actorRotaion;
	FRotator deltaRotaion;
	FRotator calculatedRotaion;


	///// Note to self, many of these definitly do not have to be public will refactor later
};
