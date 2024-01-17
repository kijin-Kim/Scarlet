// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ScarletStateMachineState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SCARLET_API UScarletStateMachineState : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnEnter();
	UFUNCTION(BlueprintNativeEvent)
	void OnTick(float DeltaTime);
	UFUNCTION(BlueprintNativeEvent)
	void OnExit();
	
};
