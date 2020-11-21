#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SolarSystemGameModeBase.generated.h"

class UGravityComponent;

UCLASS()
class SOLARSYSTEM_API ASolarSystemGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASolarSystemGameModeBase();

	void RegisterGravityComponent(UGravityComponent* component);
	FORCEINLINE float GetGravitationalConstant() const { return GravitationalConstant; }

protected:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient)
	TArray<UGravityComponent*> CelestialBodies;

	UPROPERTY(EditAnywhere)
	float GravitationalConstant = 100.0f;
};
