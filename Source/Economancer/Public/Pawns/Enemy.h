// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "Interfaces/ShotInterFace.h"
#include "AI/AICharacterTypes.h"
#include "Enemy.generated.h"

UCLASS()
class ECONOMANCER_API AEnemy : public ACharacter, public IShotInterFace
{
	GENERATED_BODY()

public:
	AEnemy();

	// getter for the AI controller 
	FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BTree; };

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// override of the shothit interface's GetShot() declaration
	virtual void GetShot(const FHitResult& hitPoint);
	

protected:
	virtual void BeginPlay() override;

	float Health = 95;
	

	// Personal Note, I keep forgetting that I have to add these in the editor, the AIController needs this 
	// Also, in the editor, replace the default AIController with NPC_AIController  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BTree;


private:

	EAIState PlayerState = EPlayerState::EPS_Uneqipped;


};
