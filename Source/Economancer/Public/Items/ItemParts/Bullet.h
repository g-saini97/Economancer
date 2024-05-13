// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"


class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ECONOMANCER_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:

	ABullet();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()

	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const float MaxDistance = 4000.0f;
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USphereComponent> BulletCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* BulletVFXComponent;  // Using Tobjectptr for this crashes the game, it compiles but don't revert it to TObjectptr again
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* BulletVFXSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileComponent;


	float GetDistanceTraveled();
	void DestoryBullet();

};
