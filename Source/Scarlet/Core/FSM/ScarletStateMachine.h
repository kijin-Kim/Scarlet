// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScarletStateMachine.generated.h"

class UScarletTransitionCondition;
class UScarletState;

UENUM()
enum class ETransitionTestPolicy : uint8
{
	OnTick,
	OnEvent
};


USTRUCT()
struct FScarletStateMachineTransitionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScarletState> TransitionStateClass;
	UPROPERTY(EditAnywhere)
	ETransitionTestPolicy TransitionTestPolicy = ETransitionTestPolicy::OnTick;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "TransitionTestPolicy == ETransitionTestPolicy::OnEvent", EditConditionHides))
	FGameplayTag TransitionEvent;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UScarletTransitionCondition>> TransitionConditions;
};

USTRUCT()
struct FScarletTransitionContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Transitions", TitleProperty = "TransitionStateClass"))
	TArray<FScarletStateMachineTransitionEntry> TransitionEntries;
};

DECLARE_MULTICAST_DELEGATE(FOnTransitionEventSignature)


/**
 * @class UScarletStateMachine
 * @brief 상태 머신 클래스. 상태와 상태의 전환을 관리합니다.
 */
UCLASS(BlueprintType, Blueprintable)
class SCARLET_API UScarletStateMachine : public UObject
{
	GENERATED_BODY()

	friend class UScarletStateMachineComponent;

public:
	void OnBeginPlay();
	void TryTransition(const FScarletStateMachineTransitionEntry& Entry);
	void OnTick(float DeltaTime);
	/**
	 * @brief 전달된 게임플레이 태그에 대한 델리게이트를 @ref OnTransitionEventDelegates에서 찾고 Broadcast합니다.
	 * @param EventTag 이벤트를 나타내는 게임플레이 태그.
	 */
	void DispatchEvent(FGameplayTag EventTag);
	UScarletStateMachineComponent* GetStateMachineComponent() const;

private:
	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& X, float& Arg);

	/**
	 * @brief 현재 상태를 전달된 상태의 클래스의 상태로 전환합니다. 
	 *
	 * @param NewStateClass 새로 전환할 상태의 클래스.
	 */
	void EnterNewState(TSubclassOf<UScarletState> NewStateClass);

	/**
	 * @brief 전달된 상태의 클래스의 전환 이벤트에 대한 델리게이트를 바인딩합니다.
	 *
	 * @details @ref 전달된 상태 클래스가 전환할 수 있는 상태와 이벤트에 대하여 검색하고, 이벤트를 키(Key), 새로운 상태로 전환하는 로직을 담고 있는 델리게이트를 값(Value)로 하여, @ref OnTransitionEventDelegates에 저장합니다.
	 * 
	 * @param StateClass 전환 이벤트에 대한 델리게이트를 바인딩할 상태의 클래스.
	 */
	void BindTransitionEvents(TSubclassOf<UScarletState> StateClass);

public:
	/**
	 * @brief 상태머신의 시작 상태.
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScarletState> RootStateClass;
	/**
	 * @brief 상태머신의 상태와 전환 조건.
	 */
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<UScarletState>, FScarletTransitionContainer> States;

protected:
	/**
	 * @brief 부모 StateMachineComponent
	 */
	UPROPERTY()
	UScarletStateMachineComponent* StateMachineComponent;

private:
	/**
	 * @brief 현재 상태
	 */
	UPROPERTY()
	TObjectPtr<UScarletState> CurrentState;
	/**
	 * 
	 */
	FScarletTransitionContainer* CurrentStateTransitions = nullptr;
	/**
	 * 
	 */
	TMap<FGameplayTag, FOnTransitionEventSignature> OnTransitionEventDelegates;
};
