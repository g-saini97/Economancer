// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/AIAnimInstance.h"
#include "AI/AIController/EnemyAIController.h"
#include "AI/NPCs/EnemyNPC.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void UAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	NPC = Cast<AEnemyNPC>(TryGetPawnOwner());// getting the AI character
	if (NPC)
	{
		AIMovement = NPC->GetCharacterMovement(); // getting the AI character's speed
		
	}
	
}


void UAIAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	// reads status from the NPC character everyframe, i hope, the Behavior tree triggers any change in this bool.
	// NOTE to Self: If NPCs aiming feels or looks Janky, start investigating at the Animations, Aimoffset , ABP and the respective Behavior Tree Tasks.
	if (AIMovement)
	{
		xySpeed = AIMovement->Velocity.Size2D();
		isAiming = NPC->GetAimBool();
		calculatedRotaion = calculateYawPitch();
		AIState = NPC->GetAIState();
		yaw = calculatedRotaion.Yaw;
		pitch = calculatedRotaion.Pitch;
	}
	
}


FRotator UAIAnimInstance::calculateYawPitch()
{
	AAIController* NPCController = Cast<AAIController>(NPC->GetController());
	AINPCController = Cast<AEnemyAIController>(NPCController);
	CurrentRotation = FRotator(pitch, yaw, 0.f);
	if (!AINPCController || !NPC)
	{
		return FRotator::ZeroRotator;
	}


	AINPCController->UpdateTargetLocation();
	FVector TargetLocation = AINPCController->GetCalculatedTargetLocation();

	FVector NPCPosition = NPC->GetActorLocation();
	FVector BoneLocation = NPC->GetMesh()->GetSocketLocation("Hand_RSocket");

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(BoneLocation, TargetLocation);
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LookAtRotation, NPC->GetActorRotation());
	
	interpedRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, DeltaRotation, GetWorld()->DeltaTimeSeconds, 15.f);
	clampedInterpRotation = FRotator(UKismetMathLibrary::ClampAngle(interpedRotation.Pitch, -89.f, 89.f), UKismetMathLibrary::ClampAngle(interpedRotation.Yaw, -89.f, 89.f), 0.f);

	return clampedInterpRotation;
}
