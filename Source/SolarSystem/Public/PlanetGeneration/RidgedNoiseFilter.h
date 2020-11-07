#pragma once

#include "PlanetGeneration/BaseNoiseFilter.h"
#include "RidgedNoiseFilter.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FRidgedNoiseFilter : public FBaseNoiseFilter
{
	GENERATED_BODY()

public:
	virtual float Evaluate(const FVector& point) override;
};
