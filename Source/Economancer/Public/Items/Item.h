// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Components/PrimitiveComponent.h"
#include "Item.generated.h"


//forward declare here
class UStaticMeshComponent;
class USphereComponent;
class USkeletalMeshComponent;


UCLASS()
class ECONOMANCER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();



protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> skeletalMesh;

private:
	

	
};
