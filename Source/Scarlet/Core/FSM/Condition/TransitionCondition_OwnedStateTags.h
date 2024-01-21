// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScarletTransitionCondition.h"
#include "TransitionCondition_OwnedStateTags.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Owning State Tags")
class SCARLET_API UTransitionCondition_OwnedStateTags : public UScarletTransitionCondition
{
	GENERATED_BODY()
public:
	virtual bool Test(UScarletStateMachine* StateMachine);

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer OwnedStateTags;
};
