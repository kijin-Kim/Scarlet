// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SCARLET_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetingComponent();
	virtual void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	void CalculateTargetActor();

	
	UFUNCTION(BlueprintCallable)
	void CalculateNewLockOnTarget(bool bToRight);
	UFUNCTION(BlueprintCallable)
	void ToggleTargeting();

	

	

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> TargetableActorClass;

private:
	/**
	 * 전제:
	 * 1. Component의 Owner는 Pawn또는 Pawn을 상속한 클래스여야 한다. (Controller에 접근하므로)
	 * 2. Owner는 SpringArmComponent를 가져야 한다. (피치에 따라 자연스러운 카메라 움직임을 만들 때, SpringArmComponent의 TargetArmLength를 조작한다.) 
	 * 3. Owner는 Camera Component를 가져야 한다. (카메라의 전방벡터를 사용한 수학적 계산을 통해 타겟팅 할 대상을 결정한다)
	 * 4. Owner의 첫번째 CameraComponent는 첫번째 SpringArmComponent에 Attach되어 있어야 한다. (2번 참고)
	 * 5. Owner는 언젠간 Controller에 의해 빙의될 것을 전제로 한다.
	 */
	UPROPERTY()
	TObjectPtr<APawn> PawnOwner;
	UPROPERTY()
	TObjectPtr<USpringArmComponent> OwnerSpringArm;
	float BaseTargetArmLength = 0.0f;
	UPROPERTY()
	TObjectPtr<UCameraComponent> OwnerCamera;


	UPROPERTY()
	TObjectPtr<AActor> TargetedActor;
	
	
	
	
};
