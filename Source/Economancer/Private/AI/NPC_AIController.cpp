// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC_AIController.h"
#include "AI/NPCs/NPCCharacter.h"
#include "AI/CoverSystem/CoverManager.h"
#include "AI/CoverSystem/CoverPoint.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "Navigation/PathFollowingComponent.h"




ANPC_AIController::ANPC_AIController()
{
	SetupPerceptionSystem();
}

void ANPC_AIController::BeginPlay()
{
	Super::BeginPlay();
	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); // getting the PLayer character to set the PLayerActor key in the blackboard
	if (Player)
	{
		SetPlayerReference(Player);
	}

	for (TActorIterator<ACoverManager> It(GetWorld()); It; ++It)
	{
		ACoverManager* FoundCoverManager = *It;
		if (FoundCoverManager)
		{
			CoverManager = FoundCoverManager;
			break;
		}
	}
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
		SightConfig->SightRadius = 2000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 200.f;
		SightConfig->PeripheralVisionAngleDegrees = 175.f;
		SightConfig->SetMaxAge(1.f);
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


/// Cover System Functions

void ANPC_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		ReleaseCurrentCoverPoint();
	}
}


FVector ANPC_AIController::FindAndReserveCoverPoint(AActor* Player)
{
	if (CoverManager)
	{
		ACoverPoint* CoverPoint = CoverManager->FindCoverPoint(GetPawn(), Player);
		if (CoverPoint)
		{
			if (CurrentCoverPoint)
			{
				CoverManager->ReleaseCoverPoint(CurrentCoverPoint);
			}
			CurrentCoverPoint = CoverPoint;
			return CoverPoint->GetActorLocation();
		}
	}
	return FVector::ZeroVector;
}

void ANPC_AIController::ReleaseCurrentCoverPoint()
{
	if (CoverManager && CurrentCoverPoint)
	{
		CoverManager->ReleaseCoverPoint(CurrentCoverPoint);
		CurrentCoverPoint = nullptr;
	}
}

void ANPC_AIController::SetPlayerReference(AActor* Player)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject("PlayerActor", Player); /// Note to self, why did i miss spell "PlayerActor"
	}
}


