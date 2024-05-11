// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstance.generated.h"


class AWeapon;
class APlayerCharacter;

UCLASS()
class ECONOMANCER_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeapon> Weapon;

	//UPROPERTY(BlueprintReadOnly, Category = "Player")
	//TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bIsFiring;


	

};
