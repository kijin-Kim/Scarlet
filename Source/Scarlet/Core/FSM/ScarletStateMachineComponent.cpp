// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletStateMachineComponent.h"

#include "DisplayDebugHelpers.h"
#include "ScarletStateMachine.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"

UScarletStateMachineComponent::UScarletStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UScarletStateMachineComponent::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.Reset();
	TagContainer.AppendTags(OwnedStateTags);
}

void UScarletStateMachineComponent::AddTag(FGameplayTag Tag)
{
	OwnedStateTags.AddTag(Tag);
}

void UScarletStateMachineComponent::AddTags(const FGameplayTagContainer& TagContainer)
{
	OwnedStateTags.AppendTags(TagContainer);
}

void UScarletStateMachineComponent::RemoveTag(FGameplayTag Tag)
{
	OwnedStateTags.RemoveTag(Tag);
}

void UScarletStateMachineComponent::RemoveTags(const FGameplayTagContainer& TagContainer)
{
	OwnedStateTags.RemoveTags(TagContainer);
}

void UScarletStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	BindHUDDebugInfoEvent();

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

void UScarletStateMachineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnBindHUDDebugInfoEvent();
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

void UScarletStateMachineComponent::BindHUDDebugInfoEvent()
{
	const APawn* PawnOwner = GetOwner<APawn>();
	if (!PawnOwner || !PawnOwner->IsPlayerControlled())
	{
		return;
	}

	OnShowDebugInfoHandle = AHUD::OnShowDebugInfo.AddLambda([](AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& X, float& Arg)
	{
		const AActor* TargetActor = HUD->GetCurrentDebugTargetActor();
		if (!TargetActor)
		{
			return;
		}

		if (UScarletStateMachineComponent* TargetStateMachineComponent = TargetActor->GetComponentByClass<UScarletStateMachineComponent>())
		{
			TargetStateMachineComponent->OnShowDebugInfo(HUD, Canvas, DebugDisplay, X, Arg);
		}
	});
}

void UScarletStateMachineComponent::UnBindHUDDebugInfoEvent()
{
	if (OnShowDebugInfoHandle.IsValid())
	{
		AHUD::OnShowDebugInfo.Remove(OnShowDebugInfoHandle);
	}
}

void UScarletStateMachineComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& X, float& Arg)
{
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	if (DebugDisplay.IsDisplayOn(TEXT("StateMachine")))
	{
		DisplayDebugManager.SetDrawColor(FColor::White);
		DisplayDebugManager.DrawString(TEXT("Owned State Machines"), X);
		X += 4.0f;
		for (UScarletStateMachine* StateMachine : StateMachines)
		{
			StateMachine->OnShowDebugInfo(HUD, Canvas, DebugDisplay, X, Arg);
		}
		X -= 4.0f;
	}
}
