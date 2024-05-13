// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCs/NPCCharacter.h"
#include "AI/NPC_AIController.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Economancer/DebugMacros.h"
#include "Kismet/KismetMathLibrary.h"


ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	if (AnimInstance && MeleeMontage_1)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Dummy Attacks"));
		AnimInstance->Montage_Play(MeleeMontage_1);
		AnimInstance->Montage_JumpToSection(FName("Punch_1"), MeleeMontage_1);
	}
}



void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (overlappingItem)
	{
		PickUpWeapon();
	}

	
}

void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCCharacter::PickUpWeapon() // right now the AI only picks up on overlap, it does not seek out these weapons
{
	TObjectPtr<AWeapon> overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon)
	{
		overlappingWeapon->Equip(GetMesh(), FName("hand_LSocket"));
		overlappingItem = nullptr; // setting ovelapping item to nullptr because it kept pointing to the weapon currently euiped and player could not pickup a new weapon if they had picked one up already.
		equippedWeapon = overlappingWeapon;
		availibleWeapons.Add(equippedWeapon);
		PlayerState = EAIState::EAIS_EquippedOneHanded;
	}
}

void ANPCCharacter::Shoot()
{
	return;
}

void ANPCCharacter::ReactToBulletHit(FHitResult Hit)
{
	Health -= 50;

	if (Health <= 0)
	{
		// Disable capsule collision and physics
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetSimulatePhysics(false);

		// Enable physics for the mesh and set animation mode to physics
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->SetAllBodiesSimulatePhysics(true);

		// Apply impulse to simulate hit reaction
		FVector ImpulseVector = (Hit.ImpactPoint - Hit.TraceStart).GetSafeNormal();
		GetMesh()->AddImpulseAtLocation(ImpulseVector * 8000.f, Hit.ImpactPoint, Hit.BoneName);
	

		// Deactivate AI controller
		ANPC_AIController* AIController = Cast<ANPC_AIController>(GetController());
		if (AIController)
		{
			AIController->StopMovement();
			AIController->Destroy();
		}
	}
}

// INterface implementations 
void ANPCCharacter::GetShot(const FHitResult& hit) /// use if bing hit with a raycasted weapon

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


}  /// use if bing hit with a raycasted weapons


int ANPCCharacter::MeleeAttack_Implementation()
{

	
	
	if (MeleeMontage_1)
	{
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && MeleeMontage_1)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("Dummy Attacks"));
			AnimInstance->Montage_Play(MeleeMontage_1);
			AnimInstance->Montage_JumpToSection(FName("Punch_1"), MeleeMontage_1);
		}
	}

	return 0;
}


