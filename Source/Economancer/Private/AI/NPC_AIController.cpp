// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC_AIController.h"
#include "AI/NPCs/NPCCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"




ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInititalizer)
{
	SetupPerceptionSystem();
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ANPCCharacter* NPC = Cast<ANPCCharacter>(InPawn))
	{
		if (UBehaviorTree* tree = NPC->GetBehaviorTree())
		{
			UBlackboardComponent* b = nullptr;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);


		}

	}
}






/// <summary>
///  PERCEPTION System related functions start here
/// </summary>
/// 
void ANPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"))); // This sets the current perception component for the system
		SightConfig->SightRadius = 1200.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;
		SightConfig->PeripheralVisionAngleDegrees = 115.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 250.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,
															&ANPC_AIController::OnTargetdetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);

	}
}

void ANPC_AIController::OnTargetdetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const character = Cast<APlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed()); // if the NPC sees a player a key is flipped in the blackboard
	}
}
