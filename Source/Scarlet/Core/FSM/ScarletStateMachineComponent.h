// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ScarletStateMachineComponent.generated.h"


class UScarletState;
class UScarletStateMachine;

DECLARE_MULTICAST_DELEGATE(FOnTransitionEventSignature)


/**
 * @class UScarletStateMachineComponent
 * 
 * @brief 상태 머신을 관리하는 매니저의 역할을 하는 컴포넌트
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SCARLET_API UScarletStateMachineComponent : public UActorComponent,  public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UScarletStateMachineComponent();
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	void AddTag(FGameplayTag Tag);
	void AddTags(const FGameplayTagContainer& TagContainer);
	void RemoveTag(FGameplayTag Tag);
	void RemoveTags(const FGameplayTagContainer& TagContainer);
	

	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 컴포넌트가 가지고 있는 모든 상태머신에게 이벤트를 전달합니다.
	 * @param EventTag 이벤트를 나타내는 게임플레이 태그.
	 */
	void DispatchEvent(FGameplayTag EventTag);


private:
	void BindHUDDebugInfoEvent();
	void UnBindHUDDebugInfoEvent();
	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& X, float& Arg);

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UScarletStateMachine>> StateMachineClasses;

private:
	UPROPERTY()
	TArray<TObjectPtr<UScarletStateMachine>> StateMachines;
	FDelegateHandle OnShowDebugInfoHandle;

	FGameplayTagContainer OwnedStateTags;
};
