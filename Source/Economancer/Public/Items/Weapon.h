// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "WeaponTypes.h"
#include "Interfaces/Animation/AnimatableWeapon.h"
#include "ItemProperties/WeaponFireModes.h"
#include "Weapon.generated.h"

class USceneComponent;
class UFieldSystemComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class ABullet;


UCLASS()
class ECONOMANCER_API AWeapon : public AItem, public IAnimatableWeapon
{
	GENERATED_BODY()
public:
	AWeapon();

	

	// Need to be public because player has to access it
	void Shoot(AController* playerController);
	void Aim(AController* playerController);
	void AimEnd();
	void Equip(TObjectPtr<USceneComponent> inParent, FName(inSocketName));
	void FireModeSelect();
	void TriggerRelease();
	void AttatchToPlayerSocket(TObjectPtr<USceneComponent> inParent, const FName& inSocketName);

	virtual bool IsFiring() const override;

protected:
	
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAttributes, meta = (AllowPrivateAccess = "true")) // the handedness of the weapon
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAttributes, meta = (AllowPrivateAccess = "true")) // FireMode Enum
	EWeaponFireMode FireMode = EWeaponFireMode::EWFM_Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponAttributes, meta = (AllowPrivateAccess = "true"))
	float fireRate;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float range = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float roundCapacity;

	float shotLimit = 0.f ;
	float shotsTaken = 0.f;

	FCollisionQueryParams traceParams;
	FHitResult shotHit;
	FHitResult aimHit;
	bool bIsHit;
	bool bCanFire = true;
	bool bShotFired = false;
	
	void ResetChamber();
	void MuzzleFlash();
	void ShellEject();
	AActor* ShootBullet(FVector Endpoint, AController* playerController);
	
	

	// utility
	void CreateFields(const FHitResult& hit);

private:

	UPROPERTY(EditAnywhere, Category = "MuzzleFlashVFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* MuzzleVfxSystem;

	UPROPERTY(EditAnywhere, Category = "MuzzleFlashVFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* MuzzleVfxComponent; // Using Tobjectptr for this crashes the game, it compiles but don't revert it to TObjectptr again

	UPROPERTY(EditAnywhere, Category = "ShellVFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ShellEjectionSystem;

	UPROPERTY(EditAnywhere, Category = "ShellVFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* ShellEjectionComponent;
	
	UPROPERTY(EditAnywhere, Category = "Bullet Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABullet> BulletType; /// So that we can choose which kind of bullet a weapon fires from within the blueprint editor of any derived weapon editor 
	
	UPROPERTY(EditAnywhere, Category = "MuzzleVFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> MuzzleComponent;

	UPROPERTY(EditAnywhere, Category = "ShellVFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ShellEjector;

	TObjectPtr<UFieldSystemComponent> Field;
	
	FTimerHandle ShootTimerHandle;

};
