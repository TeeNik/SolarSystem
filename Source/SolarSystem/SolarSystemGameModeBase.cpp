#include "SolarSystemGameModeBase.h"
#include "Gravity/GravityComponent.h"

ASolarSystemGameModeBase::ASolarSystemGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ASolarSystemGameModeBase::RegisterGravityComponent(UGravityComponent* component)
{
	CelestialBodies.Emplace(component);
}

void ASolarSystemGameModeBase::Tick(float DeltaTime)
{
	for(UGravityComponent* body : CelestialBodies)
	{
		body->UpdateVelocity(CelestialBodies, DeltaTime);
	}

	for (UGravityComponent* body : CelestialBodies)
	{
		body->UpdatePosition( DeltaTime);
	}
}
