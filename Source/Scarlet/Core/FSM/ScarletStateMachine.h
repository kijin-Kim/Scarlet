// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ScarletStateMachine.generated.h"

class UScarletStateMachineState;
/**
 * 
 */

USTRUCT()
struct FScarletStateMachineTransitionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TransitionEvent;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScarletStateMachineState> TransitionStateClass;
};

USTRUCT()
struct FScarletStateMachineTransitionContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Transitions", TitleProperty = "TransitionStateClass"))
	TArray<FScarletStateMachineTransitionEntry> StateMachineTransitionEntries;
};


UCLASS()
class SCARLET_API UScarletStateMachine : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief 상태머신의 시작 상태
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScarletStateMachineState> RootStateClass;

	/**
	 * @brief 상태머신의 상태와 전환 조건
	 */
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UScarletStateMachineState>, FScarletStateMachineTransitionContainer> States;
};
