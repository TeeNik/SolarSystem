#include "PlanetGeneration/ShapeGenerator.h"
#include "PlanetGeneration/PlanetGenerator.h"
#include "PlanetGeneration/NoiseFilterFactory.h"

UShapeGenerator::UShapeGenerator()
{
	NoiseFilterFactory = NewObject<UNoiseFilterFactory>();
}

void UShapeGenerator::Init()
{
	NoiseFilters.Empty();
	for (const FNoiseLayer& noiseLayer : Settings.NoiseLayers)
	{
		UBaseNoiseFilter* noiseFilter = NoiseFilterFactory->GetNoiseFilter(noiseLayer.NoiseSettings);
		NoiseFilters.Emplace(noiseFilter);
	}
	ResetMinMax();
}

void UShapeGenerator::ResetMinMax()
{
	MinMax.Key = FLT_MAX;
	MinMax.Value = FLT_MIN;
}

void UShapeGenerator::AddMinMax(float value)
{
	if (value > MinMax.Value)
	{
		MinMax.Value = value;
	}
	if (value < MinMax.Key)
	{
		MinMax.Key = value;
	}
}

float UShapeGenerator::CalculateScaledElevation(float unscaledElevation)
{
	float elevation = FMath::Max(0.0f, unscaledElevation);
	return Settings.PlanetRadius * (1 + elevation);
}

float UShapeGenerator::CalculateUnscaledElevation(FVector pointOnUnitSphere)
{
	float firstLayerValue = 0;
	float elevation = 0;

	if (NoiseFilters.Num() > 0)
	{
		firstLayerValue = NoiseFilters[0]->Evaluate(pointOnUnitSphere);
		if (Settings.NoiseLayers[0].Enabled)
		{
			elevation = firstLayerValue;
		}
	}

	for (int i = 1; i < NoiseFilters.Num(); ++i)
	{
		if (Settings.NoiseLayers[i].Enabled)
		{
			float mask = (Settings.NoiseLayers[i].UseFirstLayerAsMask) ? firstLayerValue : 1;
			elevation += NoiseFilters[i]->Evaluate(pointOnUnitSphere) * mask;
		}
	}
	AddMinMax(elevation);
	return elevation;
}

void UShapeGenerator::SetSettings(const TArray<FNoiseLayer>& NoiseLayers)
{
	Settings.NoiseLayers = NoiseLayers;
}