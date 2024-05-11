// Fill out your copyright notice in the Description page of Project Settings.

#include "Anims/PlayerAnimInstance.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"



void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	playerCharacter =  Cast<APlayerCharacter>(TryGetPawnOwner()); // getting the player character
	if (playerCharacter)
	{
		playerMovement =  playerCharacter->GetCharacterMovement(); // getting the  player character's speed
	}


}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


	if (playerMovement)
	{
		xySpeed = UKismetMathLibrary::VSizeXY(playerMovement->Velocity); // speed of character on the ground plane.
		isFalling = playerMovement->IsFalling();
		PlayerState = playerCharacter->GetPlayerState();
		isAiming = playerCharacter->GetAimBool();

		calculatedRotaion =  calculateYawPitch();
		yaw = calculatedRotaion.Yaw;
		pitch = calculatedRotaion.Pitch;
		
		//if (GEngine) {
			//FString TheFloatStr = FString::SanitizeFloat(yaw);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);
		//}
	}
}

FRotator UPlayerAnimInstance::calculateYawPitch()
{
	CurrentRotation = FRotator(pitch, yaw, 0.f);
	controlerRotaion =  playerCharacter->GetControlRotation();
	actorRotaion = playerCharacter->GetActorRotation();

	deltaRotaion = UKismetMathLibrary::NormalizedDeltaRotator(controlerRotaion, actorRotaion);

	interpedRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, deltaRotaion, GetDeltaSeconds(), 15.f);
	clampedInterpRotation = FRotator(UKismetMathLibrary::ClampAngle(interpedRotation.Pitch,-179.f,179.f),UKismetMathLibrary::ClampAngle(interpedRotation.Yaw, -179.f, 179.f), 0.f);

	return clampedInterpRotation;
}

