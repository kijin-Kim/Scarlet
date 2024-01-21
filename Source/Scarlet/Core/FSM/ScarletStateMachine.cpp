// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletStateMachine.h"

#include "ScarletState.h"
#include "ScarletStateMachineComponent.h"
#include "Condition/ScarletTransitionCondition.h"
#include "Engine/Canvas.h"


void UScarletStateMachine::OnBeginPlay()
{
	check(RootStateClass);
	StateMachineComponent = GetStateMachineComponent();
	check(StateMachineComponent);
	EnterNewState(RootStateClass);
}

void UScarletStateMachine::TryTransition(const FScarletStateMachineTransitionEntry& Entry)
{
	if (Entry.TransitionConditions.IsEmpty())
	{
		EnterNewState(Entry.TransitionStateClass);
		return;
	}

	for (UScarletTransitionCondition* TransitionCondition : Entry.TransitionConditions)
	{
		if (TransitionCondition && TransitionCondition->Test(this))
		{
			EnterNewState(Entry.TransitionStateClass);
			return;
		}
	}
}

void UScarletStateMachine::OnTick(float DeltaTime)
{
	check(CurrentState);
	CurrentState->OnTick(DeltaTime);


	if (CurrentStateTransitions)
	{
		for (const FScarletStateMachineTransitionEntry& Entry : CurrentStateTransitions->TransitionEntries)
		{
			if (Entry.TransitionStateClass && Entry.TransitionTestPolicy == ETransitionTestPolicy::OnTick)
			{
				TryTransition(Entry);
			}
		}
	}
}

void UScarletStateMachine::DispatchEvent(FGameplayTag EventTag)
{
	check(EventTag.IsValid());

	const FOnTransitionEventSignature* Delegate = OnTransitionEventDelegates.Find(EventTag);
	if (Delegate && Delegate->IsBound())
	{
		Delegate->Broadcast();
	}
}

UScarletStateMachineComponent* UScarletStateMachine::GetStateMachineComponent() const
{
	return StateMachineComponent ? StateMachineComponent : Cast<UScarletStateMachineComponent>(GetOuter());
}

void UScarletStateMachine::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& X, float& Arg)
{
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
	const FColor DrawColor = FColor::White;
	DisplayDebugManager.SetDrawColor(DrawColor);

	DisplayDebugManager.DrawString(FString::Printf(TEXT("%s"), *GetNameSafe(this)), X);

	X += 4.0f;
	DisplayDebugManager.DrawString(FString::Printf(TEXT("Current State: %s"), *GetNameSafe(CurrentState)), X);
	X -= 4.0f;
}

void UScarletStateMachine::EnterNewState(TSubclassOf<UScarletState> NewStateClass)
{
	if (CurrentState)
	{
		CurrentState->OnExit();

		StateMachineComponent->RemoveTag(CurrentState->StateTag);
	}
	CurrentState = NewObject<UScarletState>(this, NewStateClass);
	StateMachineComponent->AddTag(CurrentState->StateTag);
	CurrentState->OnEnter();
	BindTransitionEvents(NewStateClass);
}

void UScarletStateMachine::BindTransitionEvents(TSubclassOf<UScarletState> StateClass)
{
	OnTransitionEventDelegates.Empty();
	FScarletTransitionContainer* Transitions = States.Find(StateClass); // 전달된 상태의 클래스로부터 새롭게 전환될 상태가 없을 경우
	if (!Transitions)
	{
		return;
	}

	CurrentStateTransitions = Transitions;

	for (FScarletStateMachineTransitionEntry& Entry : CurrentStateTransitions->TransitionEntries)
	{
		if (Entry.TransitionStateClass && Entry.TransitionTestPolicy == ETransitionTestPolicy::OnEvent && Entry.TransitionEvent.IsValid())
		{
			OnTransitionEventDelegates.FindOrAdd(Entry.TransitionEvent).AddLambda([this, &Entry]()
			{
				TryTransition(Entry);
			});
		}
	}
}
