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
	NoiseAdjustments.Empty();
	for (const FNoiseLayer& noiseLayer : Settings.NoiseLayers)
	{
		FNoiseAdjustment noiseAdjustment;
		noiseAdjustment.SetSettings(noiseLayer.NoiseSettings);
		NoiseAdjustments.Emplace(noiseAdjustment);
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

	if (NoiseAdjustments.Num() > 0)
	{
		firstLayerValue = NoiseAdjustments[0].Evaluate(pointOnUnitSphere);
		if (Settings.NoiseLayers[0].Enabled)
		{
			elevation = firstLayerValue;
		}
	}

	for (int i = 1; i < NoiseAdjustments.Num(); ++i)
	{
		if (Settings.NoiseLayers[i].Enabled)
		{
			float mask = (Settings.NoiseLayers[i].UseFirstLayerAsMask) ? firstLayerValue : 1;
			elevation += NoiseAdjustments[i].Evaluate(pointOnUnitSphere) * mask;
		}
	}
	return pointOnUnitSphere * Settings.PlanetRadius * (1 + elevation);
}
