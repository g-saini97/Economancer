// Fill out your copyright notice in the Description page of Project Settings.


//// Personal Note: After a lot of pain trying to use this for the AI classes as well,
//// ended up making a seperate anim instance class for AI NPCs
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/PlayerTypes.h"
#include "PlayerAnimInstance.generated.h"


//Forward Declarations
class APlayerCharacter;
class UCharacterMovementComponent;


UCLASS()
class ECONOMANCER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	FRotator calculateYawPitch();

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<APlayerCharacter> playerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> playerMovement;

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
	EPlayerState PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "Aim Direction");
	FRotator playerCameraRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Aim Direction");
	FVector playerCameraLocation;

	/////// vars for yaw and pitch calculations or the aim offset. Do not touch (aiming will break)
	FRotator CurrentRotation;
	FRotator interpedRotation;
	FRotator clampedInterpRotation; // want rotation to stay between -90 to 90 degrees;
	FRotator controlerRotaion;
	FRotator actorRotaion;
	FRotator deltaRotaion;
	FRotator calculatedRotaion;


	///// Note to self, many fo these definitly do not have to be public will refactor later

};
