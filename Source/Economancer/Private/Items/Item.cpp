// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "Characters/PlayerCharacter.h"


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
	TObjectPtr<APlayerCharacter> playerChatacter = Cast<APlayerCharacter>(OtherActor);
	if (playerChatacter)
	{
		playerChatacter->SetOverlappingItem(this);
	}

}

void AItem::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//const FString overlapEndMessage = "AAaAAAA";
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, overlapEndMessage);
	//}

	TObjectPtr<APlayerCharacter> playerChatacter = Cast<APlayerCharacter>(OtherActor);
	if (playerChatacter)
	{
		playerChatacter->SetOverlappingItem(nullptr); //sending null ptr to the character since the character is responsible for attacthment
	}											// so that the player cannot pick it up after they have stopped overlapping with the item
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	//Delegate binding to callback onSphereBeginOverlap
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::onSphereOverlap);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::onSphereEndOverlap);

	
}


