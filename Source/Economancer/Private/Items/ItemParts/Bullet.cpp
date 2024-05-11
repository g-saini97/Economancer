// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemParts/Bullet.h"
#include "AI/NPCs/NPCCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Components/PrimitiveComponent.h" 
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "NiagaraSystem.h" 
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h" 



ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("bullet Collision"));
	// Set collision profile of the bullet component

	// Set up collision response to ignore collisions with objects of the same collision profile
	BulletCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore); // Assuming ECC_GameTraceChannel1 is the collision channel of the bullet itself
	SetRootComponent(BulletCollision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Mesh"));
	BulletMesh->SetupAttachment(BulletCollision);

	BulletVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Bullet VFX"));
	BulletVFXComponent->SetupAttachment(BulletMesh);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movememt Component"));

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	

	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::onSphereOverlap);
	//BulletCollision->OnComponentEndOverlap.AddDynamic(this, &ABullet::onSphereEndOverlap);
	
	SpawnLocation = this->GetActorLocation();
	//if (BulletVFXComponent) 
	{
		BulletVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(BulletVFXSystem, BulletVFXComponent, NAME_None, BulletVFXComponent->GetComponentLocation(), BulletVFXComponent->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::None, true);
	}
}




void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetDistanceTraveled() > MaxDistance)
	{
		DestoryBullet();
	}
	
}

void ABullet::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor exists
	if (OtherActor)
	{
		// If the other actor is an NPC character
		if (TObjectPtr<ANPCCharacter> NPCCharacter = Cast<ANPCCharacter>(OtherActor))
		{
			// React to bullet hit for NPCs
			NPCCharacter->ReactToBulletHit(SweepResult);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NPC detected"));

			// Destroy the bullet after hitting an NPC
			Destroy();
		}
	}
	else
	{
		// If the other actor is not the bullet mesh or collision, destroy the bullet
		if (OtherComp && OtherComp != BulletMesh && OtherComp != BulletCollision)
		{
			Destroy();
		}
	}
}


float ABullet::GetDistanceTraveled()
{
	return (this->GetActorLocation() - SpawnLocation).Size();
}

void ABullet::DestoryBullet()
{
	Destroy();
}


