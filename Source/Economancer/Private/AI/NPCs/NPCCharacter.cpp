// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCs/NPCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Economancer/DebugMacros.h"
#include "Kismet/KismetMathLibrary.h"


ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// if enemy collision detection or visibility is janky, start investigating here.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}


void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ANPCCharacter::GetShot(const FHitResult& hit)
{
	Health -= 50;
	DEBUG_SPHERE_SPAWNER_COLORED(hit.ImpactPoint, FColor::Green);
	if (Health <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		//GetMesh()->WakeAllRigidBodies();
		FVector ImpulseVector = (hit.ImpactPoint - hit.TraceStart).GetSafeNormal();
		GetMesh()->AddImpulseAtLocation(ImpulseVector * 8000.f, hit.ImpactPoint, hit.BoneName);
		DrawDebugLine(GetWorld(), hit.ImpactPoint, hit.TraceStart, FColor::Blue, false, 2.f);
	}


}

