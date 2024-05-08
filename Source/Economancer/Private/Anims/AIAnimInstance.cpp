// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/AIAnimInstance.h"
#include "AI/NPC_AIController.h"
#include "AI/NPCs/NPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AICharacter = Cast<ANPCCharacter>(TryGetPawnOwner());// getting the AI character
	if (AICharacter)
	{
		AIMovement = AICharacter->GetCharacterMovement(); // getting the AI character's speed
	}
}


void UAIAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (AIMovement)
	{
		xySpeed = AIMovement->Velocity.Size2D();
	}
}

FRotator UAIAnimInstance::calculateYawPitch()
{
	return FRotator();
}
