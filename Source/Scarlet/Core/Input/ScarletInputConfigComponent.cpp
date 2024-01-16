// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletInputConfigComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ScarletInputMapping.h"
#include "ScarletNativeGameplayTags.h"
#include "GameFramework/Character.h"
#include "Scarlet/Core/FSM/ScarletStateMachineComponent.h"


class UEnhancedInputLocalPlayerSubsystem;

UScarletInputConfigComponent::UScarletInputConfigComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UScarletInputConfigComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = CastChecked<ACharacter>(GetOwner());
	CharacterOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::OnOwnerCharacterControllerChanged); // 컨트롤러가 바뀔 때마다 입력 설정하는 함수를 호출합니다.
	if (AController* Controller = CharacterOwner->GetController()) // 이미 컨트롤러가 빙의(Possess)되었을 경우, 입력을 설정해야 합니다.
	{
		SetupInputOnPossess();
	}
}

void UScarletInputConfigComponent::OnOwnerCharacterControllerChanged(APawn*, AController*, AController* NewController)
{
	if (NewController) // 새로운 컨트롤러가 빙의(Possess)되지 않을 경우, 입력을 설정을 시도하면 안됩니다. 
	{
		SetupInputOnPossess();
	}
}

void UScarletInputConfigComponent::SetupInputOnPossess()
{
	const APlayerController* PlayerController = CharacterOwner->GetController<APlayerController>();
	check(PlayerController && "플레이어 컨트롤러에 의해 빙의되어야 합니다.");

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	check(Subsystem);
	Subsystem->ClearAllMappings();

	for (auto [InputMappingContext, Priority] : InputMapping->InputMappingContexts)
	{
		Subsystem->AddMappingContext(InputMappingContext, Priority);
	}


	UEnhancedInputComponent* EnhancedInputComponent = CharacterOwner->GetComponentByClass<UEnhancedInputComponent>();
	check(EnhancedInputComponent)
	EnhancedInputComponent->ClearActionBindings();

	for (auto [InputAction, InputTag] : InputMapping->InputActions)
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputActionTriggered, InputTag);
	}
}

void UScarletInputConfigComponent::OnInputActionTriggered(const FInputActionValue& InputActionValue, FGameplayTag InputTag)
{
	if (InputTag == ScarletNativeInputTags::Input_Move)
	{
		Move(InputActionValue);
	}
	else if (InputTag == ScarletNativeInputTags::Input_Look)
	{
		Look(InputActionValue);
	}
	else if (InputTag == ScarletNativeInputTags::Input_Jump)
	{
		CharacterOwner->Jump();
	}

	if(UScarletStateMachineComponent* StateMachine = CharacterOwner->GetComponentByClass<UScarletStateMachineComponent>())
	{
		StateMachine->DispatchEvent(InputTag);
	}
	
	
}

void UScarletInputConfigComponent::Move(const FInputActionValue& Value)
{
	const AController* Controller = CharacterOwner->GetController();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FVector2D MovementVector = Value.Get<FVector2D>();
		CharacterOwner->AddMovementInput(ForwardDirection, MovementVector.Y);
		CharacterOwner->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UScarletInputConfigComponent::Look(const FInputActionValue& Value)
{
	if (CharacterOwner->GetController())
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		CharacterOwner->AddControllerYawInput(LookAxisVector.X);
		CharacterOwner->AddControllerPitchInput(LookAxisVector.Y);
	}
}
