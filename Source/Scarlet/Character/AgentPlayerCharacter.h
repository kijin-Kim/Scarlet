// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentCharacter.h"
#include "AgentPlayerCharacter.generated.h"

UCLASS()
class SCARLET_API AAgentPlayerCharacter : public AAgentCharacter
{
	GENERATED_BODY()

public:
	AAgentPlayerCharacter();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	
};
