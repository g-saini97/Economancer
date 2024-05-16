// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_Dodge.h"
#include "Interfaces/BTInterfaces/DodgeInterface.h"
#include "AI/NPC_AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"



UBTTask_Dodge::UBTTask_Dodge()
{
	NodeName = TEXT("Dodge");
}

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const ShouldNotDodge = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());// dodge check by looking the the blackboardkey
	if (ShouldNotDodge)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}


	auto* const Controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	auto* const NPC = Cast<ANPCCharacter>(Controller->GetPawn());

	if (auto* const DodgeInterface = Cast<IDodgeInterface>(NPC))
	{
		if (MontageHasFinished(NPC))
		{
			DodgeInterface->Execute_DodgeBullet(NPC);
		}
		
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Dodge::MontageHasFinished(ANPCCharacter* const NPC)
{

		return NPC->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(NPC->GetDodgeMontage());
}
