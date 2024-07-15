// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BarryBHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void ABarryBHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController && PlayerOverLayClass)
		{
			PlayerOverlay = CreateWidget<UPlayerOverlay>(PlayerController, PlayerOverLayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}
