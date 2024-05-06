// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

class UFieldSystemComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ECONOMANCER_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	void Equip(TObjectPtr<USceneComponent> inParent, FName(inSocketName));
	void Shoot(AController* playerController);
	void Aim(AController* playerController);
	void AimEnd();

	// utility
	void AttatchToPlayerSocket(TObjectPtr<USceneComponent> inParent, const FName& inSocketName);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool fireMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float range =  400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float roundCapacity;


	// did not show up in the editor if these were in the private section, crashed the editor as soon as any derived BP was opned.
	UPROPERTY(EditAnywhere, Category = "MuzzleFlashVFX")
	class UNiagaraSystem* MuzzleVfxSystem;
	
	UPROPERTY(EditAnywhere, Category = "MuzzleFlashVFX")
	class UNiagaraComponent* MuzzleVfxComponent;

	UPROPERTY(EditAnywhere, Category = "ShellVFX")
	class UNiagaraSystem* ShellEjectionSystem;

	UPROPERTY(EditAnywhere, Category = "ShellVFX")
	class UNiagaraComponent* ShellEjectionComponent;

protected:
	
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CreateFields(const FHitResult& hit);

	FCollisionQueryParams traceParams;
	FHitResult shotHit;
	FHitResult aimHit;
	bool bIsHit;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Muzzle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ShellEjector;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFieldSystemComponent> Field;
	
	
	
};
