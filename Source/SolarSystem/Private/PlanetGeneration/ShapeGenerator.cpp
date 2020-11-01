#include "PlanetGeneration/ShapeGenerator.h"

UShapeGenerator::UShapeGenerator()
{
}

void UShapeGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void UShapeGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	
}

FVector UShapeGenerator::CalculatePointOnSphere(FVector pointOnUnitSphere)
{
	float firstLayerValue = 0;
	float elevation = 0;

	if (NoiseAdjustments.Num() > 0)
	{
		firstLayerValue = NoiseAdjustments[0].Evaluate(pointOnUnitSphere);
		//if (settings.noiseLayers[0].enabled)
		//{
		//	elevation = firstLayerValue;
		//}
	}

	//for (int i = 1; i < NoiseAdjustments.Num(); i++)
	for (int i = 0; i < NoiseAdjustments.Num(); i++)
	{
		//if (settings.noiseLayers[i].enabled)
		//{
		//	float mask = (settings.noiseLayers[i].useFirstLayerAsMask) ? firstLayerValue : 1;
		//	elevation += noiseFilters[i].Evaluate(pointOnUnitSphere) * mask;
		//}
		elevation += NoiseAdjustments[i].Evaluate(pointOnUnitSphere);
	}
	return pointOnUnitSphere * Radius * (1 + elevation);
}
