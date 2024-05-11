// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/WeaponAnimInstance.h"
#include "Items/Weapon.h"
#include "Interfaces/Animation/AnimatableWeapon.h"
#include "Characters/PlayerCharacter.h"

void UWeaponAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (IAnimatableWeapon* AnimatableWeapon = Cast<IAnimatableWeapon>(GetOwningActor()))
	{
		bIsFiring = AnimatableWeapon->IsFiring();
	}
	
}


