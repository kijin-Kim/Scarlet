// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ScarletInputMapping.generated.h"


class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FScarletInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};


USTRUCT(BlueprintType)
struct FScarletMappingContext
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly)
	int32 Priority;
};



/**
 * 
 */
UCLASS()
class SCARLET_API UScarletInputMapping : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "InputAction"))
	TArray<FScarletInputAction> InputActions;
	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "InputMappingContext"))
	TArray<FScarletMappingContext> InputMappingContexts; 
};
