#pragma once

#include "PlanetGeneration/BaseNoiseFilter.h"
#include "RidgedNoiseFilter.generated.h"

UCLASS()
class SOLARSYSTEM_API URidgedNoiseFilter : public UBaseNoiseFilter
{
	GENERATED_BODY()

public:
	virtual float Evaluate(const FVector& point) override;
};
