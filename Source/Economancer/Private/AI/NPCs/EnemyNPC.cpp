// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCs/EnemyNPC.h"
#include "AI/AIController/EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapon.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemyNPC::AEnemyNPC()
{
	PrimaryActorTick.bCanEverTick = true;

    // Assiging the AI controller here so that i do not have to do it in the editor for every Enemy derived from this.
    AIControllerClass = AEnemyAIController::StaticClass();
}

void AEnemyNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (overlappingItem)
	{
		PickUpWeapon();
	}
}

void AEnemyNPC::RecieveBulletDamage(FHitResult hit)
{
	Health -= 50;

	if (Health <= 0)
	{
		// to Disable capsule collision and physics, fixes wierd post death ragdoll behaviour 
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetSimulatePhysics(false);

		// Enable physics for the mesh and set animation mode to physics
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->SetAllBodiesSimulatePhysics(true);

		// Apply impulse to simulate hit reaction
		FVector ImpulseVector = (hit.ImpactPoint - hit.TraceStart).GetSafeNormal();
		GetMesh()->AddImpulseAtLocation(ImpulseVector * 4000.f, hit.ImpactPoint, hit.BoneName);


		// Deactivate AI controller, if i dont do this the capsule compnent keeps moving and messes witht eh enemy ragdolls, also good for optimization i think.

		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		if (AIController)
		{
			AIController->StopMovement();
			AIController->Destroy();
			AIController->VacateCoverPoint();
		}

	}
}

void AEnemyNPC::PickUpWeapon()
{
	TObjectPtr<AWeapon> overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon)
	{
		overlappingWeapon->Equip(GetMesh(), FName("hand_LSocket"));
		overlappingItem = nullptr; // setting ovelapping item to nullptr because it kept pointing to the weapon currently euiped and player could not pickup a new weapon if they had picked one up already.
		equippedWeapon = overlappingWeapon;
		availibleWeapons.Add(equippedWeapon);
		EqippedState = EAIState::EAIS_EquippedOneHanded;
	}
}

void AEnemyNPC::Shoot()
{
	if (EqippedState != EAIState::EAIS_Uneqipped)
	{
		if (equippedWeapon)
		{
			AController* AIController = GetController();
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (Player && AIController)
			{
				FVector AimDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				equippedWeapon->ShootAI(AIController, AimDirection);
			}
		}
	}
}



void AEnemyNPC::ReactToBulletHit(FHitResult Hit)
{
    // Going to implement Enemy bullet dodge here, before taking damage. 

    RecieveBulletDamage(Hit);
}



/// Shot Interface Implementation
void AEnemyNPC::GetShot(const FHitResult& hitPoint)
{
 // Not doing anything for now.
}

