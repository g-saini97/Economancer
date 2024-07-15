// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI/AICharacterTypes.h"
#include "AIAnimInstance.generated.h"


class AEnemyNPC;
class UCharacterMovementComponent;
class AEnemyAIController;
class APlayerCharacter;
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

	UFUNCTION()
	FRotator calculateYawPitch();

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<AEnemyNPC> NPC;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<AEnemyAIController> AINPCController;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> AIMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<APlayerCharacter> Player;

	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	float xySpeed;// character's speed on the ground

	UPROPERTY(BlueprintReadOnly, Category = "Aim Direction");
	float yaw;

	UPROPERTY(BlueprintReadOnly, Category = "Aim Direction");
	float pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Movement");
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Combat");
	bool isAiming = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	EAIState AIState;

	/////// vars for yaw and pitch calculations or the aim offset. 
	FRotator CurrentRotation;
	FRotator interpedRotation;
	FRotator clampedInterpRotation;
	FRotator controlerRotaion;
	FRotator actorRotaion;
	FRotator deltaRotaion;
	FRotator calculatedRotaion;


	///// Note to self, much of this definitly dose not have to be public will refactor later
};
