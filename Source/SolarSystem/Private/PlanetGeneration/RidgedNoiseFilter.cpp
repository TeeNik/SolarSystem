#include "PlanetGeneration/RidgedNoiseFilter.h"
#include "PlanetGeneration/NoiseGenerator.h"

float URidgedNoiseFilter::Evaluate(const FVector& point)
{
	float noiseValue = 0;
	float frequency = NoiseSettings.BaseRoughness;
	float amplitude = 1;
	float weight = 1;

	for (int i = 0; i < NoiseSettings.NumLayers; i++)
	{
		float v = 1 - FMath::Abs(Noise->Evaluate(point * frequency + NoiseSettings.Centre));
		v *= v;
		v *= weight;
		weight = v;

		noiseValue += v * amplitude;
		frequency *= NoiseSettings.Roughness;
		amplitude *= NoiseSettings.Persistence;
	}

	//noiseValue = FMath::Max(0.0f, noiseValue - NoiseSettings.MinValue);
	noiseValue = noiseValue - NoiseSettings.MinValue;
	return noiseValue * NoiseSettings.Strength;
}
