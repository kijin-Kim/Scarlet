// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionCondition_OwnedStateTags.h"
#include "Scarlet/Core/FSM/ScarletStateMachine.h"
#include "Scarlet/Core/FSM/ScarletStateMachineComponent.h"


bool UTransitionCondition_OwnedStateTags::Test(UScarletStateMachine* StateMachine)
{
	if (!StateMachine)
	{
		return false;
	}
	const UScarletStateMachineComponent* StateMachineComponent = StateMachine->GetStateMachineComponent();
	return StateMachineComponent && StateMachineComponent->HasAllMatchingGameplayTags(OwnedStateTags);
}
