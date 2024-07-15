// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ShotInterFace.h"
#include "AI/AICharacterTypes.h"
#include "EnemyNPC.generated.h"

class UBlackBoardComponent;
class UBehaviorTree;
class UBehaviorTreeComponent;
class AItem;
class AWeapon;
class UAnimMontage;


UCLASS()
class ECONOMANCER_API AEnemyNPC : public ACharacter, public IShotInterFace
{
	GENERATED_BODY()

public:
	AEnemyNPC();
    
    // Getters and setters, Note to Self: Try keeping all of these inline, pls.
    FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { overlappingItem = Item; };// overlapping items have to call this
    FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BehaviorTree; };
    FORCEINLINE EAIState GetAIState() const { return EqippedState; }
    FORCEINLINE bool GetAimBool() const { return bIsAiming; };
    FORCEINLINE void SetAimBoolTrue() { bIsAiming = (equippedWeapon) ? true : bIsAiming; } // Going to make a btttask to uses these setters, the animinstance needs to read these in order to play the aim animation, Note To Self.
    FORCEINLINE void SetAimBoolFalse() { bIsAiming = false; }


	virtual void Tick(float DeltaTime) override;


	// Basic ttributes and properties , will be adding more, i think.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health = 100.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Combat");
    bool bIsAiming = false;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void RecieveBulletDamage(FHitResult hit);

    UFUNCTION(BlueprintCallable, Category = "Animation")
    void PlayHitMontage(FName SectionName); // had to make it const , crashed otherwise, NOTE TO SELF: Look into this later

    UFUNCTION(BlueprintCallable, Category = "Health")
    void PickUpWeapon();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Shoot();


    // Behavior tree related components
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

    // Function used by the bullet's on hit overlap events.
    void ReactToBulletHit(FHitResult Hit);

    // Interface implementations here , the original Testing gun was ray casted, also usefull if i add an acutal raycasted gun in the game.
    virtual void GetShot(const FHitResult& hitPoint) override;


protected:
	virtual void BeginPlay() override;

private:

    EAIState EqippedState = EAIState::EAIS_Uneqipped;

    UPROPERTY(VisibleInstanceOnly)
    TObjectPtr<AItem> overlappingItem;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<AWeapon> equippedWeapon;

    UPROPERTY(VisibleAnywhere)
    TArray<AWeapon*> availibleWeapons;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* HitMontage;


};
