// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScarletAgentCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Scarlet/Core/Scarlet.h"


//////////////////////////////////////////////////////////////////////////
// AScarletCharacter

AScarletAgentCharacter::AScarletAgentCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void AScarletAgentCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const UCapsuleComponent* Capsule = GetCapsuleComponent();
	const FVector MeshLocation = Capsule->GetComponentLocation() - GetActorUpVector() * Capsule->GetScaledCapsuleHalfHeight();
	GetMesh()->SetWorldLocation(MeshLocation);
	GetMesh()->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void AScarletAgentCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector Acceleration = GetCharacterMovement()->GetCurrentAcceleration();
	const FVector2D Acceleration2D = {Acceleration.X, Acceleration.Y};
	const FVector2D Direction = Acceleration2D.GetSafeNormal();

	const FVector2D NewVelocity = Direction * DesiredSpeed;
	GetCharacterMovement()->Velocity.X = NewVelocity.X;
	GetCharacterMovement()->Velocity.Y = NewVelocity.Y;

	const float AccelerationRatio = Acceleration2D.Length() / GetCharacterMovement()->GetMaxAcceleration();
	DesiredSpeed = AccelerationRatio > RunThreshold ? GetCharacterMovement()->MaxWalkSpeed * RunSpeedRate : GetCharacterMovement()->MaxWalkSpeed * WalkSpeedRate;
}
