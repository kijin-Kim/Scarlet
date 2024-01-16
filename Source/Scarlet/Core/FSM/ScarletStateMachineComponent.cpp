// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletStateMachineComponent.h"

#include "ScarletStateMachine.h"
#include "ScarletStateMachineState.h"

UScarletStateMachineComponent::UScarletStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UScarletStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	if (StateMachine && StateMachine->RootStateClass)
	{
		EnterNewState(StateMachine->RootStateClass);
	}
}


void UScarletStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (StateMachine && CurrentState)
	{
		CurrentState->OnTick();
	}
}

void UScarletStateMachineComponent::DispatchEvent(FGameplayTag EventTag)
{
	if (!StateMachine || !EventTag.IsValid())
	{
		return;
	}

	const FOnTransitionEventSignature* Delegate = OnTransitionEventDelegates.Find(EventTag);
	if (Delegate && Delegate->IsBound())
	{
		Delegate->Broadcast();
	}
}


void UScarletStateMachineComponent::EnterNewState(TSubclassOf<UScarletStateMachineState> NewStateClass)
{
	if (!StateMachine || !NewStateClass)
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


void UScarletStateMachineComponent::BindTransitionEvents(TSubclassOf<UScarletStateMachineState> NewStateClass)
{
	OnTransitionEventDelegates.Empty();
	FScarletStateMachineTransitionContainer* Transitions = StateMachine->States.Find(NewStateClass);
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
