// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "Interfaces/BTInterfaces/CombatInterface.h" 
#include "Interfaces/BTInterfaces/DodgeInterface.h"
#include "Interfaces/ShotInterFace.h"
#include "AI/AICharacterTypes.h"
#include "NPCCharacter.generated.h"

class AItem;
class AWeapon;
class UAnimMontage;

UCLASS()
class ECONOMANCER_API ANPCCharacter : public ACharacter, public IShotInterFace, public ICombatInterface, public IDodgeInterface
{
	GENERATED_BODY()

public:
	ANPCCharacter();

	// getter for the AI controller 
	FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BTree; };
	FORCEINLINE EAIState GetAIState() const { return PlayerState; }
	FORCEINLINE UAnimMontage* GetMontage() const { return MeleeMontage; }; // make this getAttackMontage later, Note to Self
	FORCEINLINE UAnimMontage* GetDodgeMontage() const { return DodgeMontage; };
	FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { overlappingItem = Item; }
	FORCEINLINE bool GetAimBool() const { return bIsAiming; };
	FORCEINLINE bool IsAGurad() const { return bIsGuard; };
	FORCEINLINE bool IsAPatrol() const { return bIsPatrol; };

	//setters
	FORCEINLINE void SetAimBoolTrue() { bIsAiming = true; }
	FORCEINLINE void SetAimBoolFalse() { bIsAiming = false; }


	// Reaction to the Environment and player functions
	bool CanDodge()const; // if this were inline, it breaks the dodge systemm
	void PickUpWeapon();


	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Function used by the bullets on hit overlap event.
	void ReactToBulletHit(FHitResult Hit);
	void RecieveDamageFromBullet(FHitResult Hit);

	// override of the shothit interface's GetShot() declaration // for ray cast shooting
	virtual void GetShot(const FHitResult& hitPoint) override;

	// override of the CombatInterface interface's MeleeAttack() declaration // for meleeattacks dirven by the Behavior tree
	int MeleeAttack_Implementation() override; // Do not bulid/compile without defining such Implementable functions

	// override for the DodgeInterface's function
	int DodgeBullet_Implementation() override;
	
	void PlayDodgeMontage();
	
	UFUNCTION(BlueprintCallable) // Need to call this in the Anim BP
	void DodgeEnd();

protected:
	virtual void BeginPlay() override;
	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"));
	float Health = 115.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"));
	float NumberOfDodges = 4;
	bool bIsAbleToDodge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"));
	bool Dodger = false;
	
	//Conditions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Combat", meta = (AllowPrivateAccess = "true"));
	bool bIsAiming = false;

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

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAIActionState ActionState = EAIActionState::EAIAS_NotBusy;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> overlappingItem;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> equippedWeapon;

	UPROPERTY(VisibleAnywhere)
	TArray<AWeapon*> availibleWeapons;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage;
};