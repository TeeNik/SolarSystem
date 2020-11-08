#pragma once

#include "PlanetGeneration/NoiseSettings.h"
#include "BaseNoiseFilter.generated.h"

class UNoiseGenerator;

UCLASS()
class SOLARSYSTEM_API UBaseNoiseFilter : public UObject
{
	GENERATED_BODY()

public:
	UBaseNoiseFilter();

	FORCEINLINE void SetSettings(const FNoiseSettings& settings) { NoiseSettings = settings; }
	virtual float Evaluate(const FVector& point) { return 0; }

protected:
	UPROPERTY(EditAnywhere)
	FNoiseSettings NoiseSettings;

	UPROPERTY(Transient)
	UNoiseGenerator* Noise;
	
};
