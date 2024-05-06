// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"

// Sets default values
ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	//Setting up components
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	flyerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	flyerMesh->SetupAttachment(GetRootComponent());
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 300.f;
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphere->SetupAttachment(GetRootComponent());



	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

//Movement function definitions
void ABird::Move(const FInputActionValue& value)
{
	const float directionValue = value.Get<float>();
	if (Controller && directionValue != 0.f)
	{
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, directionValue);
	}
}

void ABird::Look(const FInputActionValue& value)
{
	const FVector2D lookAxisValue = value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(lookAxisValue.X);
		AddControllerPitchInput(lookAxisValue.Y);
	}
}

void ABird::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString othername = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Yellow, othername);
	}
}


void ABird::BeginPlay()
{
	Super::BeginPlay();

	//Delegate binding to callback onSphereBeginOverlap
	sphere->OnComponentBeginOverlap.AddDynamic(this, &ABird::onSphereOverlap);


	// Enhanced Input system setup
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(inputMapContext,0);
		}
	}
}


void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// casting to enhanced input component 
	if (TObjectPtr<UEnhancedInputComponent> enhancedComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &ABird::Look);
		enhancedComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &ABird::Move);

	}

}

