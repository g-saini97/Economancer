// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "Interfaces/BTInterfaces/CombatInterface.h" 
#include "Interfaces/ShotInterFace.h"
#include "AI/AICharacterTypes.h"
#include "NPCCharacter.generated.h"

class AItem;
class AWeapon;
class UAnimMontage;

UCLASS()
class ECONOMANCER_API ANPCCharacter : public ACharacter, public IShotInterFace, public ICombatInterface
{
	GENERATED_BODY()

public:
	ANPCCharacter();

	// getter for the AI controller 
	FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BTree; };
	FORCEINLINE EAIState GetAIState() const { return PlayerState; }
	FORCEINLINE UAnimMontage* GetMontage() const { return MeleeMontage_1; };



	FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { overlappingItem = Item; }
	FORCEINLINE bool GetAimBool() const { return isAiming; };
	FORCEINLINE bool IsAGurad() const { return bIsGuard; };
	FORCEINLINE bool IsAPatrol() const { return bIsPatrol; };



	// Reaction to the Environment and player functions
	void PickUpWeapon();
	void Shoot();
	void Chase();


	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Function used by the bullets on hit overlap event.
	void ReactToBulletHit(FHitResult Hit);

	// override of the shothit interface's GetShot() declaration // for ray cast shooting
	virtual void GetShot(const FHitResult& hitPoint) override;

	// override of the CombatInterface interface's GetShot() declaration // for ray cast shooting
	int MeleeAttack_Implementation() override; // Do not bulid/compile without defining such Implementable functions

protected:
	virtual void BeginPlay() override;
	// Stats
	float Health = 95;


	//Conditions
	bool isAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Parameters", meta = (AllowPrivateAccess = "true"));
	bool bIsGuard = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Parameters", meta = (AllowPrivateAccess = "true"));
	bool bIsPatrol = false;



	// Personal Note, I keep forgetting that I have to add these in the editor, the AIController needs this 
	// Also, in the editor, replace the default AIController with NPC_AIController  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BTree;


private:

	EAIState PlayerState = EAIState::EAIS_Uneqipped;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> overlappingItem;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> equippedWeapon;

	UPROPERTY(VisibleAnywhere)
	TArray<AWeapon*> availibleWeapons;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeMontage_1;
};
