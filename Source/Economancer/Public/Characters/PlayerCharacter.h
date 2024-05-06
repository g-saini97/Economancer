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
class UInputMappingContext;
class UInputAction;
class AItem;
class AWeapon;




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
	FORCEINLINE EPlayerState GetPlayerState() const { return PlayerState; }
	FORCEINLINE bool GetAimBool() const { return isAiming; };

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
	
	bool doOnceShoot = true;
	bool doOnceAim = true;
	bool isAiming;
	bool bIsHolstered = false;
	FRotator cameraRotation; //to be seen by the anim instance




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
	FCollisionQueryParams traceParams; // params for the line trace (Note for later: Check documentation for what these mystery params are. )
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> inputMapContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> sprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> pickUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> attackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> aimAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> shootAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> holsterAction;
	
	//Movement function declarations 
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void SprintStart(const FInputActionValue& value);
	void SprintEnd(const FInputActionValue& value);
	void Pickup(const FInputActionValue& value);
	void AttackStart(const FInputActionValue & value);
	void AttackEnd(const FInputActionValue& value);
	void AimStart(const FInputActionValue& value);
	void AimEnd (const FInputActionValue& value);
	void CustomJump(const FInputActionValue& value);
	void CustomJumpEnd(const FInputActionValue& value);
	void Holster(const FInputActionValue& value);
	void UnHolster();// is a sister function to holster, uses the same input.
	
	
	//Movement and Utility related funcition declarations
	void controllerOrientationLockChecker();
	void wallJumpChecker();
	void wallSlideJump(FHitResult wallHitResult);

	bool canAim();
	bool canShoot();
	

	
private:

	//PLAYER STATE ENUM IS HERE
	
	EPlayerState PlayerState = EPlayerState::EPS_Uneqipped;

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


};
