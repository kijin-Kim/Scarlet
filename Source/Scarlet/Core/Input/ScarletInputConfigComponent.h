// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "ScarletInputConfigComponent.generated.h"


class UScarletInputMapping;

/**
 * @class UScarletInputConfigComponent
 * @brief 입력과 로직을 매핑하는 역할을 합니다.
 * @details 컴포넌트를 가지고 있는 캐릭터가 플레이어 컨트롤러에 의해 빙의(Possess)될 때, 입력과 로직을 매핑합니다.
 * @remark 오너(Owner)는 캐릭터이어야 하고, 플레이어 컨트롤러에 의해 빙의(Possess)되어야 합니다.   
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SCARLET_API UScarletInputConfigComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScarletInputConfigComponent();
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOwnerCharacterControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);
	void SetupInputOnPossess();
	void OnInputActionTriggered(const FInputActionValue& InputActionValue, FGameplayTag InputTag);
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UScarletInputMapping> InputMapping;
	
private:
	/** Character 타입의 오너(Owner)의 캐시된 값 */
	UPROPERTY()
	ACharacter* CharacterOwner;

	

	
};
