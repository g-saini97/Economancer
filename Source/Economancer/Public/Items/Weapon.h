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
class AMagazine;




UCLASS()
class ECONOMANCER_API AWeapon : public AItem, public IAnimatableWeapon
{
	GENERATED_BODY()

public:
	AWeapon();

	FORCEINLINE virtual bool IsFiring() const { return bShotFired; };
	FORCEINLINE float GetRoundsInInventory() { return roundsInInventory;}
	FORCEINLINE float GetLoadedRounds() { return loadedRounds;}


	// Many of these do not need to be public, will refactor them to private later.
	void Shoot(AController* playerController);
	void ShootAI(AController* AIController, FVector AimDirection);// the NPC Character class needs to be able to access this, the npc state trees need to accsess this as well so that the NPCs can shoot back
	void StartAIShooting(AController* AIController, FVector AimDirection);
	void StopAIShooting();
	void Aim(AController* playerController);
	void AimEnd();
	void Equip(TObjectPtr<USceneComponent> inParent, FName(inSocketName));
	void FireModeSelect();
	void TriggerRelease();
	void ReloadWeapon(); // NOTE TO SELF, any fresh gun picked up by the player will have no loadedRounds and will need gto reload after pickup.
	void DetachAndDropMagazine();
	void AttachNewMagazine();
	void AttatchToPlayerSocket(TObjectPtr<USceneComponent> inParent, const FName& inSocketName);
	void UpdatePlayerHUD();

	// Gun Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDealt = 35.f; // default damage is the lowest damage any gun can deal, change this later in the editor. 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes", meta = (AllowPrivateAccess = "true")) // the handedness of the weapon
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes", meta = (AllowPrivateAccess = "true")) // FireMode Enum
	EWeaponFireMode FireMode = EWeaponFireMode::EWFM_Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes", meta = (AllowPrivateAccess = "true"))
	float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float MaxBulletSpread = 2.0f;  // Maximum angle in degrees for bullet spread

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float range = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float  roundsCapacity;  // This and below are required for the reloading and the bullet counting to function properly

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float  roundsOnPickUp = 20.f; // will dictate how many rounds the player picks up when they pick the weapon up from the floor.  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float roundsInInventory; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float  magCapacity = 18.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float loadedRounds ;  // these shall not exceed magCapacity during gameplay

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	FVector MagMeshLocation;

protected:
	
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

	float shotLimit = 10.f ;
	float shotsTaken = 0.f;

	FCollisionQueryParams traceParams;
	FHitResult shotHit;
	FHitResult aimHit;
	bool bIsHit;
	bool bCanFire = true;
	bool bShotFired = false;

	// Important for the fiering process
	void ResetChamber();
	void MuzzleFlash();
	void ShellEject();
	AActor* ShootBullet(float Damage,FVector Endpoint, AController* Controller);
	FVector AddBulletSpread(FVector AimDirection);

	// utility
	void CreateFields(const FHitResult& hit);
	void AIShootTick();

private:
	// For testing purposes, i have given them the allow private access specifier , all of them do not need that , will change that later

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* MuzzleVfxSystem;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* MuzzleVfxComponent; // Using Tobjectptr for this crashes the game, it compiles but don't revert it to TObjectptr again

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ShellEjectionSystem;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* ShellEjectionComponent;
	
	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABullet> BulletType; /// So that we can choose which kind of bullet a weapon fires from the editor. 
	
	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> MuzzleComponent;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> MagazineComponent;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ShellEjector;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MagazineMesh;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> MagazineType; // so that different types of guns can have a different magazine class attatched , each magazine classes can be derived from the Amagazine class and be made different, this is very similar to how im handeling bullets. 

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	FVector AIAimDirection;

	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	AController* AIOwnerController;



	TObjectPtr<UFieldSystemComponent> Field;
	
	FTimerHandle ShootTimerHandle;

};
