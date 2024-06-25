// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/AIAnimInstance.h"
#include "AI/AIController/EnemyAIController.h"
#include "AI/NPCs/EnemyNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AICharacter = Cast<AEnemyNPC>(TryGetPawnOwner());// getting the AI character
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

	// reads the aim status from the NPC character everyframe, i hope, the StateTree is responsibel for setting it
	// If NPCs aiming feels or looks Janky, start investigating at the State Tree and the respective State Tree Task.
	if (AICharacter)
	{
		isAiming = AICharacter->GetAimBool();
	}

}

FRotator UAIAnimInstance::calculateYawPitch()
{
	return FRotator();
}
