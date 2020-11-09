#pragma once

#include "PlanetGeneration/BaseNoiseFilter.h"
#include "NoiseFilter.generated.h"

UCLASS()
class SOLARSYSTEM_API UNoiseFilter : public UBaseNoiseFilter
{
	GENERATED_BODY()

public:
	virtual float Evaluate(const FVector& point) override;
};
