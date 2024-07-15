// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPCs/EnemyNPC.h"
#include "AI/AIController/EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapon.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"



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

void AEnemyNPC::PickUpWeapon()
{
	TObjectPtr<AWeapon> overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon)
	{
		overlappingWeapon->Equip(GetMesh(), FName("Hand_RSocket"));
		overlappingItem = nullptr; // setting ovelapping item to nullptr because it kept pointing to the weapon currently euiped and player could not pickup a new weapon if they had picked one up already.
		equippedWeapon = overlappingWeapon;
		availibleWeapons.Add(equippedWeapon);
		EqippedState = EAIState::EAIS_EquippedOneHanded; // for now the equip state is hardcoded, will make the ewquipped weapon responsible for this later.
	}
}

void AEnemyNPC::Shoot()
{
	if (EqippedState != EAIState::EAIS_Uneqipped && equippedWeapon)
	{
		if (equippedWeapon)
		{
			AController* AIController = GetController();
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (Player && AIController)
			{
				FVector MuzzleLocation = GetMesh()->GetBoneLocation("Hand_R"); // Adjust socket name as needed
				FVector PlayerLocation = Player->GetActorLocation();

				// Perform a line trace from NPC's viewpoint to player's location
				FVector ViewpointLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f); // Adjust if needed for NPC's eyes height
				FVector TraceEnd = PlayerLocation;

				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(this);

				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ViewpointLocation, TraceEnd, ECC_Visibility, Params);
				if (bHit)
				{
					PlayerLocation = HitResult.ImpactPoint;
				}

				FVector AimDirection = (PlayerLocation - MuzzleLocation).GetSafeNormal();
				equippedWeapon->ShootAI(AIController, AimDirection);

				// Debuging to to check the aim math
				//if (GEngine)
				//{
				//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AimDirection: %s"), *AimDirection.ToString()));
				//	DrawDebugLine(GetWorld(), MuzzleLocation, MuzzleLocation + AimDirection * 1000.f, FColor::Red, false, 1.f, 0, 1.f);
				//}
			}
		}
	}
}



void AEnemyNPC::ReactToBulletHit(FHitResult Hit)
{
	// Note To Self: Add nore behaviors for reacting to bullets hitting npcs, for now, just taking damage
    RecieveBulletDamage(Hit);
}

void AEnemyNPC::RecieveBulletDamage(FHitResult hit)
{
	Health -= 50;
	
	if (!bIsDead)
	{
		/*const FVector Fward = GetActorForwardVector();
		const FVector ImpactPointFlattenedToZ(hit.ImpactPoint.X, hit.ImpactPoint.Y, GetActorLocation().Z); // doing this so that the angle remians parallel to the pawn's oreintation, any angle that goes up or down i s useless to us.
		const FVector ToHit = (ImpactPointFlattenedToZ - GetActorLocation()).GetSafeNormal();

		//Note To Self:  dot product gives the cos theta, have to solve for theta , find the arc cosine in the math lib somewhere ?? 
		double cosTheta =  FVector::DotProduct(Fward, ToHit);
		// there is
		double Theta =  FMath::Acos(cosTheta);
		// rads to degs
		Theta = FMath::RadiansToDegrees(Theta);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Theta : %f"), Theta));
		}
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), (GetActorLocation() + GetActorForwardVector() * 50.f), 8.f, FColor::Blue, 2.f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), (GetActorLocation() + ToHit* 50.f), 8.f, FColor::Yellow, 2.f);
		*/

		PlayHitMontage(FName("HitBack"));
	}

	if (Health <= 0)
	{
		bIsDead = true;
		// to Disable capsule collision and physics, fixes wierd post death ragdoll behaviour 
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetSimulatePhysics(false);

		// Enable physics for the mesh and set animation mode to physics
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->SetAllBodiesSimulatePhysics(true);

		// Apply impulse to simulate hit reaction
		FVector ImpulseVector = (hit.ImpactPoint - hit.TraceStart).GetSafeNormal();
		GetMesh()->AddImpulseAtLocation(ImpulseVector * 1500.f, hit.ImpactPoint, hit.BoneName);


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

void AEnemyNPC::PlayHitMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(FName("HitBack"), HitMontage);
	}
}

/// Shot Interface Implementation
void AEnemyNPC::GetShot(const FHitResult& hitPoint)
{
 // Not doing anything for now.
}

