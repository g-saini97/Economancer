// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DodgeBullet.generated.h"

/// <summary>
/// Rename this whole Service later , Note To Self
/// </summary>
UCLASS()
class ECONOMANCER_API UBTService_DodgeBullet : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_DodgeBullet();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
