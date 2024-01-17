// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ScarletStateMachineComponent.generated.h"


class UScarletStateMachineState;
class UScarletStateMachine;

DECLARE_MULTICAST_DELEGATE(FOnTransitionEventSignature)


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SCARLET_API UScarletStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScarletStateMachineComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DispatchEvent(FGameplayTag EventTag);

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UScarletStateMachine>> StateMachineClasses;

private:
	UPROPERTY()
	TArray<TObjectPtr<UScarletStateMachine>> StateMachines;
};
