#pragma once

#include "PlanetGeneration/NoiseSettings.h"
#include "NoiseAdjustment.generated.h"

class UNoiseGenerator;

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseAdjustment
{
	GENERATED_BODY()

public:
	FNoiseAdjustment();

	void SetSettings(const FNoiseSettings& settings);
	float Evaluate(FVector point);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FNoiseSettings NoiseSettings;

	UNoiseGenerator* Noise;
};
