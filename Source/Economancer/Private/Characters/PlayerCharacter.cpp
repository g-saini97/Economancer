// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "DrawDebugHelpers.h"



APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	defaultSpeedOffset = 0.3f;
	moveSpeedOffset = 0.3f; // is being changed through the Sprint() function
	sprintSpeedOffset = 10.f ;  // is being accessed by the Sprint() function


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	//Setting up components
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 400.f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced input subsystem setup
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(inputMapContext, 0);
		}
	}


}

/// TICK IS HERE 
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerState != EPlayerState::EPS_Uneqipped && isAiming   ) // for ze aim
	{
		equippedWeapon->Aim(this->GetController());
	}


	controllerOrientationLockChecker(); // stops player form roating while not grounded
	wallJumpChecker();
	

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// casting to enhanced input component 
	if (TObjectPtr<UEnhancedInputComponent> enhancedComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		enhancedComponent->BindAction(sprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SprintStart);
		enhancedComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintEnd);
		enhancedComponent->BindAction(pickUpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pickup);
		enhancedComponent->BindAction(attackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::AttackStart);
		enhancedComponent->BindAction(attackAction, ETriggerEvent::Completed, this, &APlayerCharacter::AttackEnd);
		enhancedComponent->BindAction(aimAction, ETriggerEvent::Triggered, this, &APlayerCharacter::AimStart);
		enhancedComponent->BindAction(aimAction, ETriggerEvent::Completed, this, &APlayerCharacter::AimEnd);
		enhancedComponent->BindAction(shootAction, ETriggerEvent::Triggered, this, &APlayerCharacter::AttackStart);
		enhancedComponent->BindAction(shootAction, ETriggerEvent::Completed, this, &APlayerCharacter::AttackEnd);


		// bindings to prevent contineous jumping from the space bar being held down
		enhancedComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		enhancedComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::CustomJumpEnd);
		enhancedComponent->BindAction(jumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CustomJump);
		enhancedComponent->BindAction(holsterAction, ETriggerEvent::Completed, this, &APlayerCharacter::Holster);


	}

}






///////////////////////////////////////////////////// Custom Functions start here.


void APlayerCharacter::SprintStart(const FInputActionValue& value)
{
	moveSpeedOffset = sprintSpeedOffset;
}
// to set moveSpeedOffset back to default if sprnt is let go.
void APlayerCharacter::SprintEnd(const FInputActionValue& value)
{
	moveSpeedOffset = defaultSpeedOffset;
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	const FVector2D moveDirection = value.Get<FVector2D>(); // gets the axis 2d 
	const FRotator rotation = Controller->GetControlRotation(); // 
	const FRotator yawRotation(0.f, rotation.Yaw, 0.f);

	const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X); // getting the forward vector
	const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y); // gettng the right vector
	FVector normalizedMovementDirection = forward * moveDirection.Y + right * moveDirection.X;
	normalizedMovementDirection.Normalize(); // normalizing the movement vector for proper diagonal movement
	//AddMovementInput(forward, moveDirection.Y * moveSpeedOffset); 
	//AddMovementInput(right, moveDirection.X * moveSpeedOffset);
	AddMovementInput(normalizedMovementDirection, moveSpeedOffset);// normalized the movement direction to fix the diagonal movment issue

}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	const FVector2D lookVector = value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(lookVector.Y);
}

// Interaction Logic/Systems
void APlayerCharacter::Pickup(const FInputActionValue& value)
{
	TObjectPtr<AWeapon> overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon)
	{
		overlappingWeapon->Equip(GetMesh(), FName("Hand_RSocket"));
		overlappingItem = nullptr; // setting ovelapping item to nullptr because it kept pointing to the weapon currently euiped and player could not pickup a new weapon if they had picked one up already.
		equippedWeapon = overlappingWeapon;
		availibleWeapons.Add(equippedWeapon);
		PlayerState = EPlayerState::EPS_EquippedOneHanded;
	}
}

void APlayerCharacter::AimStart(const FInputActionValue& value)
{
	if (canAim())
	{
		isAiming = true;
		doOnceAim = false;
	}
}

void APlayerCharacter::AimEnd(const FInputActionValue& value)
{
	isAiming = false;
	doOnceAim = true;
	if (PlayerState != EPlayerState::EPS_Uneqipped)
	{
		equippedWeapon->AimEnd();

	}
}

void APlayerCharacter::AttackStart(const FInputActionValue& value)
{

	if (canShoot())
	{
		isAiming = true;
		equippedWeapon->Shoot(this->GetController());
		doOnceShoot = false;
	}
}

void APlayerCharacter::AttackEnd(const FInputActionValue& value)
{
	doOnceShoot = true;
}

void APlayerCharacter::CustomJump(const FInputActionValue& value)
{
	if (bwallHitLeft || bwallHitRight)
	{
		bDoWallJump = true;
	}
}


void APlayerCharacter::CustomJumpEnd(const FInputActionValue& value)
{
	ACharacter::StopJumping();

	bDoWallJump = false;
}

void APlayerCharacter::Holster(const FInputActionValue& value)
{
	if (!bIsHolstered && equippedWeapon)
	{
		equippedWeapon->AttatchToPlayerSocket(GetMesh(), FName("HandGunHolster_R"));
		equippedWeapon = nullptr;
		PlayerState = EPlayerState::EPS_Uneqipped;
		bIsHolstered = true;
	}
	else
	{
		UnHolster();
	}

}

void APlayerCharacter::UnHolster()
{
	if (bIsHolstered && !equippedWeapon)
	{
		equippedWeapon = availibleWeapons[0];
		equippedWeapon->Equip(GetMesh(), FName("Hand_RSocket"));
		bIsHolstered = false;
		PlayerState = EPlayerState::EPS_EquippedOneHanded;
	}
}

/// Movement related functions, look furthur for utililty

void APlayerCharacter::controllerOrientationLockChecker()
{
	// logic to stop the player from rotating while not groundee 
	if (this->GetCharacterMovement()->IsFalling())
	{
		this->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	if (!this->GetCharacterMovement()->IsFalling())
	{
		this->GetCharacterMovement()->bOrientRotationToMovement = true;

	}
}

void APlayerCharacter::wallJumpChecker()
{

	if (this->GetCharacterMovement()->IsFalling())
	{
		
		FVector movementDirection = this->GetVelocity().GetSafeNormal();
		FVector rightVector = this->GetActorRightVector();
		traceStart = this->GetActorLocation();
		traceEndRight = traceStart + rightVector * 65.f;
		traceEndLeft = traceStart + (-rightVector) * 65.f;
		// Right line trace
		bwallHitRight = GetWorld()->LineTraceSingleByChannel(wallHitRight, traceStart, traceEndRight, ECC_GameTraceChannel1, traceParams);	
		DrawDebugLine(GetWorld(), traceStart, traceEndRight, FColor::Red, false, 2.0f);

		// Left Line trace
		bwallHitLeft = GetWorld()->LineTraceSingleByChannel(wallHitLeft, traceStart, traceEndLeft, ECC_GameTraceChannel1, traceParams);
		DrawDebugLine(GetWorld(), traceStart, traceEndLeft, FColor::Red, false, 2.0f);
		
		if (bwallHitLeft)
		{
			wallSlideJump(wallHitLeft);
		}
		if (bwallHitRight)
		{
			wallSlideJump(wallHitRight);
		}
		
	}
	else
	{
		return;
	}
}

void APlayerCharacter::wallSlideJump(FHitResult wallHitResult)
{
	
	DrawDebugBox(GetWorld(), wallHitResult.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.f);
	currentVelocity = this->GetVelocity();
	FVector impulseVector = (this->GetActorUpVector() * 550.f) + ((wallHitResult.Normal + GetActorForwardVector()) * 750.f);
	FVector newPlayerRotation = wallHitResult.Normal;
	
	if (bcanrotate)
	{
		this->SetActorRotation(UKismetMathLibrary::RInterpTo(this->GetActorRotation(), newPlayerRotation.Rotation(), GetWorld()->GetDeltaSeconds(), 10.f));
		bcanrotate = false;
	
	}

	if (currentVelocity.Z < 0)
	{
			GetCharacterMovement()->Velocity = UKismetMathLibrary::VInterpTo_Constant(currentVelocity, FVector(0, 0, -110), GetWorld()->DeltaRealTimeSeconds, 1200.f); ///
	}

	if (bDoWallJump)
	{
		GetCharacterMovement()->Velocity = impulseVector;
		bcanrotate = true;
		bDoWallJump = false;
	}
}


// General Utility


bool APlayerCharacter::canAim() // if aimimg is janky, start Investigating here.
{
	if (PlayerState != EPlayerState::EPS_Uneqipped &&  doOnceAim  )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool APlayerCharacter::canShoot()
{
	if (PlayerState != EPlayerState::EPS_Uneqipped && doOnceShoot && equippedWeapon )
	{
		return true;
	}
	else
	{
		return false;
	}
}