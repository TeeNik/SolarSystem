#pragma once

#include "PlanetGeneration/BaseNoiseFilter.h"
#include "NoiseFilter.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseFilter : public FBaseNoiseFilter
{
	GENERATED_BODY()

public:
	virtual float Evaluate(const FVector& point) override;
};
