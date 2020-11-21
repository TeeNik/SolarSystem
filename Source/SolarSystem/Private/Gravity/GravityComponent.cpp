#include "Gravity/GravityComponent.h"
#include "../SolarSystemGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

UGravityComponent::UGravityComponent()
{
}

void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentVelocity = InitialVelocity;

	ASolarSystemGameModeBase* GM = Cast<ASolarSystemGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GM)
	{
		G = GM->GetGravitationalConstant();
		GM->RegisterGravityComponent(this);
	}
}

void UGravityComponent::UpdateVelocity(TArray<UGravityComponent*> allBodies, float deltaTime)
{
	const FVector ownerPos = GetOwner()->GetActorLocation();
	for(UGravityComponent* otherBody : allBodies)
	{
		if(otherBody != this)
		{
			FVector otherPos = otherBody->GetOwner()->GetActorLocation();
			float sqrDist = FVector::DistSquared(otherPos, ownerPos);
			FVector forceDir = otherPos - ownerPos;
			forceDir.Normalize();
			FVector force = forceDir * G * Mass * otherBody->Mass / sqrDist;
			FVector acceleration = force / Mass;
			CurrentVelocity += acceleration * deltaTime;
		}
	}
}

void UGravityComponent::UpdatePosition(float deltaTime)
{
	FVector ownerPos = GetOwner()->GetActorLocation();
	GetOwner()->SetActorLocation(ownerPos + CurrentVelocity * deltaTime);
	
	DrawDebug();
}

void UGravityComponent::DrawDebug()
{
	if(ShouldDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), 100, 12, FColor::Green, 3, 1);
	}
}