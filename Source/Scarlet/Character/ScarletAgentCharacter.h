// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ScarletAgentCharacter.generated.h"


/**
 * @brief 게임 내 인간 형태의 Agent의 Base class
 */
UCLASS(config=Game)
class AScarletAgentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AScarletAgentCharacter();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
	float RunSpeedRate = 1.0f;
	UPROPERTY(EditAnywhere)
	float WalkSpeedRate = 0.3f;
	UPROPERTY(EditAnywhere)
	float RunThreshold = 0.9f;

private:
	float DesiredSpeed = 0.0f;
};
