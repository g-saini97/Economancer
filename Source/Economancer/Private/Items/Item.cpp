// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "Characters/PlayerCharacter.h"
#include "AI/NPCs/EnemyNPC.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(sphere);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mesh->SetupAttachment(GetRootComponent());

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeltalMesh"));
	skeletalMesh->SetupAttachment(GetRootComponent());
}



void AItem::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//const FString othername = OtherActor->GetName();
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, othername);
	//}
	
	if (PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->SetOverlappingItem(this);
	}
	if (TObjectPtr<AEnemyNPC> NPC= Cast<AEnemyNPC>(OtherActor)) // Note to self, Store the reference to the NPC character as well , later
	{
		NPC->SetOverlappingItem(this);
	}
}

void AItem::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//const FString overlapEndMessage = "AAaAAAA";
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, overlapEndMessage);
	//}
	if (TObjectPtr<APlayerCharacter> playerChatacter = Cast<APlayerCharacter>(OtherActor))
	{
		playerChatacter->SetOverlappingItem(nullptr); //sending null ptr to the character since the character is responsible for attacthment
	}											// so that the player cannot keep pickicg up an item after they have stopped overlapping with the item.
	if (TObjectPtr<AEnemyNPC> NPC = Cast<AEnemyNPC>(OtherActor))
	{
		NPC->SetOverlappingItem(nullptr);
	}
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	//Delegate binding to callback onSphereBeginOverlap
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::onSphereOverlap);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::onSphereEndOverlap);

	
}


