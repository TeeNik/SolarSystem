#include "PlanetGeneration/ShapeGenerator.h"
#include "PlanetGeneration/PlanetGenerator.h"

UShapeGenerator::UShapeGenerator()
{
}

void UShapeGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	IsInited = false;
	APlanetGenerator* planet = Cast<APlanetGenerator>(GetOwner());
	planet->CallMeshGeneration();
}

void UShapeGenerator::Init()
{
	NoiseFilters.Empty();
	for (const FNoiseLayer& noiseLayer : Settings.NoiseLayers)
	{
		FNoiseFilter noiseFilter;
		noiseFilter.SetSettings(noiseLayer.NoiseSettings);
		NoiseFilters.Emplace(noiseFilter);
	}
	IsInited = true;
}

FVector UShapeGenerator::CalculatePointOnSphere(FVector pointOnUnitSphere)
{
	if(!IsInited)
	{
		Init();
	}

	float firstLayerValue = 0;
	float elevation = 0;

	if (NoiseFilters.Num() > 0)
	{
		firstLayerValue = NoiseFilters[0].Evaluate(pointOnUnitSphere);
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
			elevation += NoiseFilters[i].Evaluate(pointOnUnitSphere) * mask;
		}
	}
	return pointOnUnitSphere * Settings.PlanetRadius * (1 + elevation);
}
