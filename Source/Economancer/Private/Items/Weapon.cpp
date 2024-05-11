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
	
	Field = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("Field"));
	//Field->SetupAttachment(GetRootComponent());
}

// overlap declarations
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
		shotLimit = 9.f;
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
		shotLimit = 0.f;
		FireMode = EWeaponFireMode::EWFM_Single;
		break;
	}
}


void AWeapon::Shoot(AController* playerController)

{
	

	if (bCanFire)
	{
		bCanFire = false;
		FVector cameraLocation;
		FRotator cameraRotation;

		playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

		FVector MuzzleLocation = MuzzleComponent->GetComponentLocation();
		FVector DirectionToCenterOfScreen = cameraRotation.Vector();

		FHitResult HitResult;
		FVector EndPoint = MuzzleLocation + DirectionToCenterOfScreen * range;

		// Perform a line trace to determine the actual end point where the bullet will hit
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, EndPoint, ECC_Visibility);
		if (bHit)
		{
			EndPoint = HitResult.ImpactPoint;
		}
		if (shotsTaken <= shotLimit)
		{
			MuzzleFlash();
			ShellEject();
			ShootBullet(EndPoint, playerController);
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

AActor* AWeapon::ShootBullet(FVector Endpoint, AController* playerController)
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = playerController->GetPawn();
	bShotFired = true;
	AActor* SpawnActor = GetWorld()->SpawnActor<ABullet>(BulletType, MuzzleComponent->GetComponentLocation(), (Endpoint - MuzzleComponent->GetComponentLocation()).Rotation(), SpawnParams);
	++shotsTaken;
	 // the weapon animinstance needs to see this.
	return SpawnActor;
	
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

void AWeapon::Aim(AController* playerController)
{

	FVector cameraLocation;
	FRotator cameraRotation;

	playerController->GetPlayerViewPoint(cameraLocation, cameraRotation);

	FVector shotTraceEnd = cameraLocation + (cameraRotation.Vector() * range);
	if (GetWorld()->LineTraceSingleByChannel(aimHit, MuzzleComponent->GetComponentLocation(), shotTraceEnd, ECC_Visibility, traceParams))
	{
		mesh->SetWorldLocation(aimHit.Location);

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
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	RootComponent->AttachToComponent(inParent, transformRules, inSocketName); // Always atatch the root component while equiping, if i do it to the mesh, it decoupls from the weapon and leaves it behind when the player moves.
}

bool AWeapon::IsFiring() const
{
	return bShotFired;
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
