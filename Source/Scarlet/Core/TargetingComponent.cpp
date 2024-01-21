// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingComponent.h"

#include "Scarlet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = CastChecked<APawn>(GetOwner());
	OwnerCamera = GetOwner()->GetComponentByClass<UCameraComponent>();
	check(OwnerCamera);
	OwnerSpringArm = GetOwner()->GetComponentByClass<USpringArmComponent>();
	check(OwnerSpringArm);
	
	BaseTargetArmLength = OwnerSpringArm->TargetArmLength;
}

void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!TargetedActor || !PawnOwner || !PawnOwner->GetController() || !OwnerCamera || !OwnerSpringArm)
	{
		return;
	}

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCamera->GetComponentLocation(), TargetedActor->GetActorLocation());
	const FRotator NewRotation = FMath::RInterpTo(PawnOwner->GetController()->GetControlRotation(), LookAtRotation, GetWorld()->GetTimeSeconds(),
	                                              3.0f);
	PawnOwner->GetController()->SetControlRotation({0.0f, NewRotation.Yaw, 0.0f});
	const float DeltaPitch = FMath::Abs(NewRotation.Pitch);
	OwnerSpringArm->TargetArmLength = BaseTargetArmLength + DeltaPitch * 20.0f;
	DrawDebugSphere(GetWorld(), TargetedActor->GetActorLocation(), 100.0f, 12, FColor::Yellow);
}

void UTargetingComponent::CalculateTargetActor()
{
	// TODO: Component 바깥에서 주입
	TArray<AActor*> TargetCandidates;
	UGameplayStatics::GetAllActorsOfClass(this, TargetableActorClass, TargetCandidates);
	TargetCandidates.RemoveAllSwap(
		[this](const AActor* Actor)
		{
			if (Actor == PawnOwner)
			{
				return true;
			}
			return false;
		});


	TargetedActor = nullptr;
	if (OwnerCamera)
	{
		float MaxDot = -1.0f;
		for (AActor* Candidate : TargetCandidates)
		{
			const FVector ToCandidate = UKismetMathLibrary::GetDirectionUnitVector(PawnOwner->GetActorLocation(), Candidate->GetActorLocation());
			const float Dot = OwnerCamera->GetForwardVector().Dot(ToCandidate);
			if (Dot > MaxDot)
			{
				FHitResult HitResult;
				const TArray<AActor*> ActorsToIgnore;
				UKismetSystemLibrary::SphereTraceSingle(this, OwnerCamera->GetComponentLocation(), Candidate->GetActorLocation(), 50.0f,
				                                        TraceTypeQuery1,
				                                        false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);

				
				if (HitResult.bBlockingHit && HitResult.GetActor() == Candidate)
				{
					TargetedActor = Candidate;
					MaxDot = Dot;
				}
			}
		}
	}
}

void UTargetingComponent::CalculateNewLockOnTarget(bool bToRight)
{
	if (!TargetedActor)
	{
		return;
	}
	
	// TODO: Component 바깥에서 주입
	TArray<AActor*> TargetCandidates;
	UGameplayStatics::GetAllActorsOfClass(this, TargetableActorClass, TargetCandidates);
	TargetCandidates.RemoveAllSwap(
		[this](const AActor* Actor)
		{
			if (Actor == PawnOwner || Actor == TargetedActor)
			{
				return true;
			}
			return false;
		});


	AActor* NewTargetedActor = nullptr;

	FVector CameraForwardProj = OwnerCamera->GetForwardVector();
	CameraForwardProj.Z = 0.0f;

	float MaxDot = -1.0f;
	for (AActor* Candidate : TargetCandidates)
	{
		const FVector ToCandidate = UKismetMathLibrary::GetDirectionUnitVector(PawnOwner->GetActorLocation(), Candidate->GetActorLocation());
		if (Candidate != TargetedActor)
		{
			const FVector Crossed = OwnerCamera->GetForwardVector().Cross(ToCandidate);
			if (bToRight ? Crossed.Z > 0 : Crossed.Z < 0)
			{
				const float Dot = CameraForwardProj.Dot(ToCandidate);
				if (Dot > MaxDot)
				{
					FHitResult HitResult;
					TArray<AActor*> ActorsToIgnore;
					ActorsToIgnore.Add(TargetedActor);
					UKismetSystemLibrary::SphereTraceSingle(this, OwnerCamera->GetComponentLocation(), Candidate->GetActorLocation(), 50.0f,
															TraceTypeQuery1,
															false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
					if (HitResult.bBlockingHit && HitResult.GetActor() == Candidate)
					{
						NewTargetedActor = Candidate;
						MaxDot = Dot;
					}
				}
			}
		}
	}

	if (NewTargetedActor)
	{
		TargetedActor = NewTargetedActor;
	}
}



void UTargetingComponent::ToggleTargeting()
{
	if (TargetedActor)
	{
		TargetedActor = nullptr;
		return;
	}

	CalculateTargetActor();
}
