#include "PlanetGeneration/NoiseFilter.h"
#include "PlanetGeneration/NoiseGenerator.h"

float FNoiseFilter::Evaluate(const FVector& point)
{
	float noiseValue = 0;
	float frequency = NoiseSettings.BaseRoughness;
	float amplitude = 1;
	
	for (int i = 0; i < NoiseSettings.NumLayers; i++)
	{
		float v = Noise->Evaluate(point * frequency + NoiseSettings.Centre);
		noiseValue += (v + 1) * .5f * amplitude;
		frequency *= NoiseSettings.Roughness;
		amplitude *= NoiseSettings.Persistence;
	}
	
	noiseValue = FMath::Max(0.0f, noiseValue - NoiseSettings.MinValue);
	return noiseValue * NoiseSettings.Strength;
}
