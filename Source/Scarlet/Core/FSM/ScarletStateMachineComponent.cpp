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

	StateMachines.Reserve(StateMachineClasses.Num());
	for (TSubclassOf<UScarletStateMachine> StateMachineClass : StateMachineClasses)
	{
		if (StateMachineClass)
		{
			StateMachines.Add(NewObject<UScarletStateMachine>(this, StateMachineClass));
		}
	}

	for (UScarletStateMachine* StateMachine : StateMachines)
	{
		StateMachine->OnBeginPlay();
	}
}


void UScarletStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (UScarletStateMachine* StateMachine : StateMachines)
	{
		StateMachine->OnTick(DeltaTime);
	}
}

void UScarletStateMachineComponent::DispatchEvent(FGameplayTag EventTag)
{
	for (UScarletStateMachine* StateMachine : StateMachines)
	{
		StateMachine->DispatchEvent(EventTag);
	}
}
