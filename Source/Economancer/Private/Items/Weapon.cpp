// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Components/SceneComponent.h" 
#include "Items/ItemParts/Bullet.h"
#include "Interfaces/ShotInterFace.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Field/FieldSystemComponent.h"
#include "NiagaraSystem.h" 
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;


	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	MuzzleComponent->SetupAttachment(GetRootComponent()); 

	ShellEjector = CreateDefaultSubobject<USceneComponent>(TEXT("ShellEjector"));
	ShellEjector->SetupAttachment(GetRootComponent());
	
	if (RootComponent == nullptr) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("no root for weapon"));
		}
	}
	MuzzleVfxComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlashVfxComponent"));
	MuzzleVfxComponent->SetupAttachment(GetRootComponent());
	
	ShellEjectionComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShellEjection component"));
	ShellEjectionComponent->SetupAttachment(GetRootComponent());
	
	MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMeshComponent"));
	MagazineMesh->SetupAttachment(GetRootComponent());
	
	MagazineComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Magazine Location"));
	MagazineComponent->SetupAttachment(GetRootComponent());

	MagMeshLocation = MagazineMesh->GetComponentLocation();
	
	Field = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field"));
	//Field->SetupAttachment(GetRootComponent());
}

// overlap declarations, using supers 
void AWeapon::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::onSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::onSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}




////////////////////////////////////////////////// Custom Functions start here.


void AWeapon::Equip(TObjectPtr<USceneComponent> inParent, FName(inSocketName))
{
	AttatchToPlayerSocket(inParent, inSocketName);
	
	if (sphere)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // so that the weapon stops ovelapping after pickup.
	}
}

void AWeapon::FireModeSelect()
{
	switch (FireMode)
	{
	case EWeaponFireMode::EWFM_Burst:
		shotLimit = 2.f;
		FireMode = EWeaponFireMode::EWFM_Auto;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Burst"));
		break;
	case EWeaponFireMode::EWFM_Auto:
		shotLimit = magCapacity - 1.f;
		FireMode = EWeaponFireMode::EWFM_Single;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Auto"));
		break;
	case EWeaponFireMode::EWFM_Single:
		shotLimit = 0.f;
		FireMode = EWeaponFireMode::EWFM_Burst;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Semi-Auto"));
		break;
	default:
		//shotLimit = 0.f;
		//FireMode = EWeaponFireMode::EWFM_Single;
		break;
	}
}


void AWeapon::Shoot(AController* playerController)

{
	

	if (bCanFire && loadedRounds >= 1)
	{
		bCanFire = false;
		FVector cameraLocation;
		FRotator cameraRotation;

		playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

		FVector MuzzleLocation = MuzzleComponent->GetComponentLocation();
		FVector DirectionToCenterOfScreen = cameraRotation.Vector();

		FHitResult HitResult;
		FVector EndPoint = cameraLocation + DirectionToCenterOfScreen * range;

		// Perform a line trace to determine the actual end point where the bullet will hit
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndPoint, ECC_Visibility);
		if (bHit)
		{
			EndPoint = HitResult.ImpactPoint;
		}
		if (shotsTaken <= shotLimit)
		{
			ShootBullet(DamageDealt,EndPoint, playerController);
			loadedRounds--; // bullet count goes down, gotta implement bullet pickups 
		}
		const float FireRate = 0.05f;
		GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &AWeapon::ResetChamber, FireRate, false);
		/*
		FVector shotTraceEnd = cameraLocation + (cameraRotation.Vector() * range);
		bIsHit = GetWorld()->LineTraceSingleByChannel(shotHit, Muzzle->GetComponentLocation(), shotTraceEnd, ECC_Visibility, traceParams);
		if (bIsHit)
		{
			DrawDebugLine(GetWorld(), Muzzle->GetComponentLocation(), shotHit.ImpactPoint, FColor::Cyan, false, 2.0f);
			CreateFields(shotHit);

			if (shotHit.GetActor())
			{
				IShotInterFace* shotInterface = Cast<IShotInterFace>(shotHit.GetActor()); // casting to the Shot interface
				if (shotInterface)
				{
					shotInterface->GetShot(shotHit);
				}

			}

			if (shotHit.GetComponent()->GetCollisionObjectType() == ECC_Destructible)
			{
				CreateFields(shotHit);
				FVector impulseVector = (shotHit.ImpactPoint - Muzzle->GetComponentLocation()).GetSafeNormal();
				shotHit.GetComponent()->AddImpulseAtLocation(impulseVector * 1000, shotHit.ImpactPoint);// adding impulse to whatever is hit
				DrawDebugBox(GetWorld(), shotHit.ImpactPoint, FVector(5, 5, 5), FColor::Red, false, 2.0f);

			}
		}*/
	}
	else if (loadedRounds <= 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("RELOAD")));
		}
	}

}

void AWeapon::ShootAI(AController* AIController, FVector AimDirection)
{
	if (bCanFire)
	{
		bCanFire = false;
		FVector MuzzleLocation = MuzzleComponent->GetComponentLocation();

		// Add randomness to the aim direction
		AimDirection = AddBulletSpread(AimDirection);

		FVector EndPoint = MuzzleLocation + AimDirection * range;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndPoint, ECC_Visibility);
		if (bHit)
		{
			EndPoint = HitResult.ImpactPoint;
		}

		ShootBullet(DamageDealt,EndPoint, AIController);

		const float FireRate = 0.0005f;
		GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &AWeapon::ResetChamber, FireRate, false);
	}
}

// brings in the direction of the target when the npc will be shooting from the NPCAIController
void AWeapon::StartAIShooting(AController* AIController, FVector AimDirection)
{
	AIOwnerController = AIController;
	AIAimDirection = AimDirection;

	ShootAI(AIController, AIAimDirection);
}

void AWeapon::StopAIShooting()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("STOP Shooting called"));

	shotsTaken = 0.f;
	//GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

// Redundant, remove if not needed by the end
void AWeapon::AIShootTick()
{
	if (AIOwnerController)
	{
		ShootAI(AIOwnerController, AIAimDirection);
	}
}

void AWeapon::MuzzleFlash()
{
	if (MuzzleVfxComponent) // Muzzle flash happens here.
	{
		MuzzleVfxComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleVfxSystem, MuzzleComponent, NAME_None, MuzzleComponent->GetComponentLocation(), MuzzleComponent->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None, true);
	}
}

void AWeapon::ShellEject()
{
	if (ShellEjectionComponent)// Shell Ejection happens here.
	{
		ShellEjectionComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(ShellEjectionSystem, ShellEjector, NAME_None, ShellEjector->GetComponentLocation(), ShellEjector->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None, true);
	}
}

AActor* AWeapon::ShootBullet(float Damage,FVector Endpoint, AController* Controller)
{

	bShotFired = true;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Controller->GetPawn();

	FVector MuzzleLocation = MuzzleComponent->GetComponentLocation();
	FRotator BulletRotation = (Endpoint - MuzzleLocation).Rotation();

	// Ensure the spawn location is clear
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Controller->GetPawn());

	bool bClear = !GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, MuzzleLocation + BulletRotation.Vector() * 10.f, ECC_Visibility, Params);
	if (bClear)
	{
		AActor* SpawnActor = GetWorld()->SpawnActor<ABullet>(BulletType, MuzzleLocation, BulletRotation, SpawnParams);
		if (SpawnActor)
		{
			++shotsTaken;

			ABullet* SpawnedBullet = Cast<ABullet>(SpawnActor);
			if (SpawnedBullet)
			{
				MuzzleFlash();
				ShellEject();
				SpawnedBullet->SetDamageDealt(DamageDealt);
			}
			return SpawnActor;
		}
	}
	else
	{
		bShotFired = false; // no shot fired if bullet spawn point is invalid,  this bool stops the weapon's anim instance from animating
	}
	return nullptr;
}

FVector AWeapon::AddBulletSpread(FVector AimDirection)
{
	FRotator AimRot = AimDirection.Rotation();
	AimRot.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
	AimRot.Pitch += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
	return AimRot.Vector();
}

void AWeapon::ResetChamber()
{
	bShotFired = false;
	bCanFire = true;
}

void AWeapon::TriggerRelease()
{
	shotsTaken = 0.f;
}

void AWeapon::ReloadWeapon()
{
	
	float roundsNeeded = magCapacity - loadedRounds; // to check if the player actually needs to reload, otherwise spamming reload causes anim spam even at full magazines

	if (roundsInInventory > 0 && roundsNeeded > 0) 
	{
		float roundsToReload = FMath::Min(roundsNeeded, roundsInInventory); // the amount of rounds needed for the reload, in case the rounds in inventory are less that the rounds required to fill the magazine or something 

		loadedRounds += roundsToReload; // Add the rounds to the loaded rounds

		roundsInInventory -= roundsToReload; // Subtracting rounds from the inventory


		// Debug message for testing
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Reloaded %f rounds. LoadedRounds: %f, RoundsInInventory: %f"), roundsToReload, loadedRounds, roundsInInventory));
		}

		// Detach and drop the current magazine
		DetachAndDropMagazine();

		// Attach a new magazine
		AttachNewMagazine();
	}
	else
	{
		// Debug message for testing
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cannot reload, either inventory is empty or magazine is full."));
		}
	}
}

void AWeapon::DetachAndDropMagazine()
{
	if (MagazineMesh)
	{
		// Detach the magazine from the weapon
		MagazineMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		// Spawn a new actor for the dropped magazine
		if (MagazineType)
		{
			FActorSpawnParameters SpawnParams;
			FVector SpawnLocation = MagazineMesh->GetComponentLocation();
			FRotator SpawnRotation = MagazineMesh->GetComponentRotation();

			GetWorld()->SpawnActor<AActor>(MagazineType, SpawnLocation, SpawnRotation, SpawnParams);
		}

		// Hide the current magazine mesh
		MagazineMesh->SetVisibility(false);
	}
}

void AWeapon::AttachNewMagazine()
{
	if (MagazineMesh)
	{
		// Reattach a new magazine to the weapon
		MagazineMesh->AttachToComponent(MagazineComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

		MagazineMesh->SetRelativeScale3D(FVector(1.375, 1.5,1.5)); // I modeled the gun a little smaller in blender by mistake, had to scale it up int he editor, these are the scale values for the model that I set in the editor, could have scalled up the ipmorted mesh but do not feel like booting up blender right now.
		// Show the new magazine mesh
		MagazineMesh->SetVisibility(true);
	}
}

void AWeapon::Aim(AController* playerController)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

	FVector shotTraceEnd = cameraLocation + (cameraRotation.Vector() * range);
	if (GetWorld()->LineTraceSingleByChannel(aimHit, MuzzleComponent->GetComponentLocation(), shotTraceEnd, ECC_Visibility, traceParams))
	{
		mesh->SetWorldLocation(aimHit.ImpactPoint);

		mesh->SetVisibility(true);
	}
	else
	{
		mesh->SetVisibility(false);
	}


}

void AWeapon::AimEnd()
{
	mesh->SetVisibility(false); // Do not use SetHidden(), crahses the game for some reason;
}

void AWeapon::AttatchToPlayerSocket(TObjectPtr<USceneComponent> inParent, const FName& inSocketName)
{
	roundsInInventory += roundsOnPickUp; // adds the pickup rounds to the inventory of the gun
	roundsOnPickUp = 0.f; // then sets the pickup amount to zero in case the player drops and picks up the weapon again.
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	RootComponent->AttachToComponent(inParent, transformRules, inSocketName); // Always atatch the root component while equiping, if i do it to the mesh, it decoupls from the weapon and leaves it behind when the player moves.
}

void AWeapon::UpdatePlayerHUD()
{
}

void AWeapon::CreateFields(const FHitResult& hit)
{
	FVector location = hit.ImpactPoint;
	FVector locationNormal = hit.ImpactNormal;
	if (Field)
	{
		Field->SetWorldLocation(location, false);
		Field->ApplyStrainField(true, location, 45.f, 5000000.f, 10.f);
	}
}

