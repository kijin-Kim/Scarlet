// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ScarletTransitionCondition.generated.h"

class UScarletStateMachine;


/**
 * 
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, CollapseCategories)
class SCARLET_API UScarletTransitionCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Test(UScarletStateMachine* StateMachine);
};
