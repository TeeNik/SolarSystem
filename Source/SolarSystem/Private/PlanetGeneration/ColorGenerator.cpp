#include "PlanetGeneration/ColorGenerator.h"

UColorGenerator::UColorGenerator()
{
}

UMaterialInstanceDynamic* UColorGenerator::GetPlanetMaterial()
{
	if(MaterialInstance == nullptr)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(Settings.PlanetMaterial, this);
	}
	return MaterialInstance;
}

void UColorGenerator::UpdateElevation(const TPair<float, float>& minMax)
{
	//Settings.PlanetMaterial.
}

void UColorGenerator::UpdateColors(TArray<FLinearColor>& vertexColors, const TPair<float, float>& minMax)
{
	for (int i = 0; i < vertexColors.Num(); ++i)
	{
		float value = (vertexColors[i].R - minMax.Key) / (minMax.Value - minMax.Key);
		vertexColors[i] = FVector(FMath::Max(value, 0.01f), 0, 0);;
	}
}

float UColorGenerator::BiomePercentFromPoint(const FVector& pointOnSphere)
{
	float heightPercent = (pointOnSphere.Z + 1) / 2.0f;
	float biomeIndex = 0;
	int numOfBiomes = Settings.BiomeColorSettings.Biomes.Num();

	for(int i = 0; i < numOfBiomes; ++i)
	{
		if(Settings.BiomeColorSettings.Biomes[i].StartHeight < heightPercent)
		{
			biomeIndex = i;
		} 
		else
		{
			break;
		}
	}
	return biomeIndex / FMath::Max(1, numOfBiomes - 1);
}

void UColorGenerator::BeginPlay()
{
	Super::BeginPlay();
}
