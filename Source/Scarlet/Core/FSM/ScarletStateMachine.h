// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScarletStateMachine.generated.h"

class UScarletStateMachineState;
/**
 * 
 */

USTRUCT()
struct FScarletStateMachineTransitionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag TransitionEvent;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScarletStateMachineState> TransitionStateClass;
};

USTRUCT()
struct FScarletStateMachineTransitionContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Transitions", TitleProperty = "TransitionStateClass"))
	TArray<FScarletStateMachineTransitionEntry> StateMachineTransitionEntries;
};

DECLARE_MULTICAST_DELEGATE(FOnTransitionEventSignature)


UCLASS(BlueprintType, Blueprintable)
class SCARLET_API UScarletStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void OnBeginPlay();
	void OnTick(float DeltaTime);
	void DispatchEvent(FGameplayTag EventTag);
	void EnterNewState(TSubclassOf<UScarletStateMachineState> NewStateClass);
	void BindTransitionEvents(TSubclassOf<UScarletStateMachineState> NewStateClass);

public:
	/**
	 * @brief 상태머신의 시작 상태
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScarletStateMachineState> RootStateClass;
	/**
	 * @brief 상태머신의 상태와 전환 조건
	 */
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<UScarletStateMachineState>, FScarletStateMachineTransitionContainer> States;

private:
	UPROPERTY()
	TObjectPtr<UScarletStateMachineState> CurrentState;
	TMap<FGameplayTag, FOnTransitionEventSignature> OnTransitionEventDelegates;
};
