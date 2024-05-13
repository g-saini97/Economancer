// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_IsPlayerInMeleeRange.h"
#include "AI/NPC_AIController.h"
#include "AI/NPCs/NPCCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// NPC and its AI controller
	auto* const Controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	auto* const NPC = Cast<ANPCCharacter>(Controller->GetPawn());

	// the player character
	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);


	//////// the servicse's logic.
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), NPC->GetDistanceTo(Player) <= MeleeRange);
}
