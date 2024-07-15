// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCs/NPCCharacter.h"
//#include "AI/NPC_AIController.h"
#include "Characters/PlayerCharacter.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Economancer/DebugMacros.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	bIsAiming = false; /// the AI controller for the NPC will need to be able to change this. The AI anim instance needs to be able to read this
}




void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void ANPCCharacter::ReactToBulletHit(FHitResult Hit)
{
	bool bShouldDodge = FMath::RandBool();

	if (Dodger && NumberOfDodges >= 0 &&  bShouldDodge )
	{
		NumberOfDodges--;
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Dodges left %f"), NumberOfDodges);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DebugMessage);
		}
		bIsAbleToDodge = true;
	}
	else
	{
		RecieveDamageFromBullet(Hit);
	}
}

void ANPCCharacter::RecieveDamageFromBullet(FHitResult Hit)
{
	bIsAbleToDodge = false;
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
		GetMesh()->AddImpulseAtLocation(ImpulseVector * 4000.f, Hit.ImpactPoint, Hit.BoneName);


		// Deactivate AI controller
		/*
		ANPC_AIController* AIController = Cast<ANPC_AIController>(GetController());
		if (AIController)
		{
			AIController->StopMovement();
			AIController->Destroy();
		}
		*/
	}
}



// INterface implementations 
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


int ANPCCharacter::MeleeAttack_Implementation()
{
	if (MeleeMontage )
	{
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && MeleeMontage)
		{
			
			AnimInstance->Montage_Play(MeleeMontage);
			AnimInstance->Montage_JumpToSection(FName("Punch_1"), MeleeMontage);
			
		}
	}

	return 0;
}


int ANPCCharacter::DodgeBullet_Implementation()
{
	if (ActionState == EAIActionState::EAIAS_NotBusy)
	{
		PlayDodgeMontage();
		ActionState = EAIActionState::EAIAS_Dodging; // gets changed in the Anim BP again.
	}
	
	return 0;
}

void ANPCCharacter::PlayDodgeMontage()
{
	if (DodgeMontage && NumberOfDodges >= 0)
	{
		bIsAbleToDodge = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("DodgeCalled"));

			int32 sectionSelector = FMath::RandRange(0, 1); /// increase the range when adding more sections to the montage
			FName sectionName = FName();
			switch (sectionSelector)
			{
			case 0:
				sectionName = FName("Dodge_1");
				break;
			case 1:
				sectionName = FName("Dodge_2");
				break;
			default:
				break;
			}
			AnimInstance->Montage_Play(DodgeMontage);
			AnimInstance->Montage_JumpToSection(sectionName, DodgeMontage);
		}

	}
}

void ANPCCharacter::Shoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, TEXT("NPC Character SHoot Called"));

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

void ANPCCharacter::DodgeEnd() // just changes the state of the NPC so that the it dos not keep dodging.
{
	ActionState = EAIActionState::EAIAS_NotBusy;
}


bool ANPCCharacter::CanDodge() const
{
	
	return bIsAbleToDodge;
}