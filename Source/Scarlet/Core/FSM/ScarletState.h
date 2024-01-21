// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ScarletState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SCARLET_API UScarletState : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnEnter();
	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float DeltaTime);
	UFUNCTION(BlueprintNativeEvent)
	void OnExit();

public:
	/**
	 * @brief 상태에 진입하고 나갈 때까지 Owner State Machine Component에 부여되는 게임플레이 태그.
	 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StateTag;
};
