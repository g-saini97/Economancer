// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h" 
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/PlayerTypes.h"
#include "PlayerCharacter.generated.h"

//Forward Declarations
class APlayerController;
class UPlayerOverlay;
class UInputMappingContext;
class UInputAction;
class AItem;
class AWeapon;
class UAIPerceptionStimuliSourceComponent;
class UWidgetComponent;



UCLASS()
class ECONOMANCER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//setters and getters , mostly getters
	FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { overlappingItem = Item; }
	FORCEINLINE void SetDamageTaken(float Damage) { DamageTaken = Damage; }
	FORCEINLINE EPlayerState GetPlayerState() const { return PlayerState; }
	FORCEINLINE bool GetAimBool() const { return isAiming; };
	FORCEINLINE AWeapon* GetEquppedWeapon() const{if (equippedWeapon){return equippedWeapon;}else{return nullptr; }} // in case somthing wants to know the weapon that the player is holding.

	// for reacting to damage from bullets or projectiles in general
	void ReactToBulletHit(FHitResult Hit, float Damage);
	void RecieveBulletDamage(FHitResult hit);

	// Player Attributes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Health = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Stamina = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDead = false;
;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float moveSpeedOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float sprintSpeedOffset;
	
	float defaultSpeedOffset;
	float velocity;
	bool isFalling;
	bool doOnceJump = true;
	bool doOnceDodge = true;
	bool bIsDodging = false;
	/// Will bring these bools up to standard by adding the b prefix, later.
	bool doOnceShoot = false;
	bool doOnceAim = true;
	bool isZooming;
	bool isAiming;
	bool bIsHolstered = false;
	FRotator cameraRotation; //to be seen by the anim instance


	// experimental walljumping related attributes
	bool bDoWallJump;
	bool bwallHitRight;
	bool bwallHitLeft;
	bool bcanrotate = false;
	FHitResult wallHitRight; // will be storing hit results from walls for the wall jump system;
	FHitResult wallHitLeft; // will be storing hit results from walls for the wall jump system;
	FVector currentVelocity;
	FVector traceStart; // will store the start position wor the walljump's wallcheck line trace;
	FVector traceEndRight; // for the line trace that goes right of the player
	FVector traceEndLeft; // for the line trace that goes -right.
	FRotator traceRotation; // the rotation of the player to calculate traceEnd;
	FCollisionQueryParams traceParams; // params for the line trace (Note To Self: Check documentation for what these mystery params are. )
	
	// Camera and SpringArm related vars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float springArmDefault = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float springArmZoomed = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FVector springArmZoomSocketOffset = FVector(0.f, 20.f, 15.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FVector springArmDefaultSocketOffset = FVector(0.f, 0.f, 0.F);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FRotator cameraZoomedRotation = FRotator(0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	FRotator cameraDefaultRotation = FRotator(0.f, 0.f, 0.f);


	/// Input Actions and The Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> inputMapContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> sprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> pickUpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> aimAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> shootAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> holsterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> switchFireMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ReloadAction; // I Know im breaking my own naming convention by going off of camel case a month later, should have started the project normaly , will refactor to standardize naming,, later
	
	//Input, Shooting and Movement function declarations 
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void SprintStart(const FInputActionValue& value);
	void SprintEnd(const FInputActionValue& value);
	void Pickup(const FInputActionValue& value);
	void AttackStart(const FInputActionValue & value);
	void AttackReleased(const FInputActionValue& value);
	void AttackEnd();
	void AimStart(const FInputActionValue& value);
	void ZoomFollowCamera(float DeltaTime);
	void AimEnd (const FInputActionValue& value);
	void SwitchFireMode(const FInputActionValue& value);
	void DodgeStart(const FInputActionValue& value); // experimental dodge mechanic
	void DodgeEnd(const FInputActionValue& value); // experimental dodge mechanic, bloodborne style 
	void CustomJump(const FInputActionValue& value);
	void CustomJumpEnd(const FInputActionValue& value);
	void Holster(const FInputActionValue& value);
	void UnHolster();// is a sister function to "Holster", no Finputaction needed.
	void ReloadEquippedWeapon(const FInputActionValue& value);
	
	
	//Movement and Utility related funcition declarations
	void controllerOrientationLockChecker(); // locks player rotation when not grounded, they can still aim and shoot any direction but not spin around midair or change fall trajectory. 
	void wallJumpChecker();
	void wallSlideJump(FHitResult wallHitResult);

	bool canAim();
	bool canShoot();
	
	
	// Perception system functions here

	/// for the perception system of the NPCs

	TObjectPtr<UAIPerceptionStimuliSourceComponent> stimulusSource;

	void SetupStimulusSource();

	
private:
	
	EPlayerState PlayerState = EPlayerState::EPS_Uneqipped;

	UPROPERTY(VisibleAnywhere)
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> springArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> camera;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> overlappingItem;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AWeapon> equippedWeapon;

	UPROPERTY(VisibleAnywhere)
	TArray<AWeapon*> availibleWeapons;

	UPROPERTY(VisibleAnywhere)
	float DamageTaken = 0.f; // Bullets hitting the player should inpart the damage, so that damage can be different depending of different kinds of weapons/Bullets

};
