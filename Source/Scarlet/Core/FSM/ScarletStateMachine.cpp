// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletStateMachine.h"

#include "ScarletStateMachineState.h"


void UScarletStateMachine::OnBeginPlay()
{
	if (RootStateClass)
	{
		EnterNewState(RootStateClass);
	}
}

void UScarletStateMachine::OnTick(float DeltaTime)
{
	if (CurrentState)
	{
		CurrentState->OnTick(DeltaTime);
	}
}

void UScarletStateMachine::DispatchEvent(FGameplayTag EventTag)
{
	if (!EventTag.IsValid())
	{
		return;
	}

	const FOnTransitionEventSignature* Delegate = OnTransitionEventDelegates.Find(EventTag);
	if (Delegate && Delegate->IsBound())
	{
		Delegate->Broadcast();
	}
}

void UScarletStateMachine::EnterNewState(TSubclassOf<UScarletStateMachineState> NewStateClass)
{
	if (!NewStateClass)
	{
		return;
	}

	if (CurrentState)
	{
		CurrentState->OnExit();
	}
	CurrentState = NewObject<UScarletStateMachineState>(this, NewStateClass);
	CurrentState->OnEnter();


	BindTransitionEvents(NewStateClass);
}

void UScarletStateMachine::BindTransitionEvents(TSubclassOf<UScarletStateMachineState> NewStateClass)
{
	OnTransitionEventDelegates.Empty();
	FScarletStateMachineTransitionContainer* Transitions = States.Find(NewStateClass);
	if (!Transitions)
	{
		return;
	}

	for (const auto [EventTag, TransitionTargetState] : Transitions->StateMachineTransitionEntries)
	{
		if (EventTag.IsValid() && TransitionTargetState)
		{
			OnTransitionEventDelegates.FindOrAdd(EventTag).AddLambda([this, TransitionTargetState]()
			{
				EnterNewState(TransitionTargetState);
			});
		}
	}
}
