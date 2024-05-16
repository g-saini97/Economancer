// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_DodgeBullet.h"
#include "AI/NPC_AIController.h"
#include "AI/NPCs/NPCCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DodgeBullet::UBTService_DodgeBullet()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Should the NPC Dodge");
}

void UBTService_DodgeBullet::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* const Controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
    if (!Controller)
    {
        return;
    }

    auto* const NPC = Cast<ANPCCharacter>(Controller->GetPawn());
    if (!NPC)
    {
        return;
    }

    const bool bcanDodge = NPC->CanDodge(); // Call the function in NPC class to determine if NPC needs to dodge
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), bcanDodge);
    }
}