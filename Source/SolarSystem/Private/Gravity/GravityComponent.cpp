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
		GM->RegisterGravityComponent(this);
	}
}

void UGravityComponent::UpdateVelocity(TArray<UGravityComponent*> allBodies, float deltaTime)
{
	const float G = 100.0f;
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

			UE_LOG(LogTemp, Log, TEXT("%s: %f %s %s %s"), *GetOwner()->GetName(), sqrDist , *forceDir.ToString(), *force.ToString(), *acceleration.ToString());
			CurrentVelocity += acceleration * deltaTime;
		}
	}
}

void UGravityComponent::UpdatePosition(float deltaTime)
{
	FVector ownerPos = GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Log, TEXT("%s: %s"), *GetOwner()->GetName(), *CurrentVelocity.ToString());
	GetOwner()->SetActorLocation(ownerPos + CurrentVelocity * deltaTime * SpeedMultiplier);

	DrawDebug();
}

void UGravityComponent::DrawDebug()
{
	if(ShouldDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), 100, 12, FColor::Green, 3, 1);
	}
}