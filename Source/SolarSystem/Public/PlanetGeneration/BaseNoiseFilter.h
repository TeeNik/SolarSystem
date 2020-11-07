#pragma once

#include "PlanetGeneration/NoiseSettings.h"
#include "BaseNoiseFilter.generated.h"

class UNoiseGenerator;

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FBaseNoiseFilter
{
	GENERATED_BODY()

public:
	FBaseNoiseFilter();

	FORCEINLINE void SetSettings(const FNoiseSettings& settings) { NoiseSettings = settings; }
	virtual float Evaluate(const FVector& point) { return 0; }

protected:
	UPROPERTY(EditAnywhere)
	FNoiseSettings NoiseSettings;

	UPROPERTY(Transient)
	UNoiseGenerator* Noise;
	
};
