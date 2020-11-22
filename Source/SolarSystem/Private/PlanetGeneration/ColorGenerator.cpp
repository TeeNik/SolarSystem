#include "PlanetGeneration/ColorGenerator.h"
#include "PlanetGeneration/NoiseFilterFactory.h"
#include "PlanetGeneration/BaseNoiseFilter.h"

UColorGenerator::UColorGenerator()
{
	NoiseFilterFactory = NewObject<UNoiseFilterFactory>();
}

void UColorGenerator::Init()
{
	NoiseFilter = NoiseFilterFactory->GetNoiseFilter(Settings.NoiseSettings);
}

float UColorGenerator::BiomePercentFromPoint(const FVector& pointOnSphere)
{
	float heightPercent = (pointOnSphere.Z + 1) / 2.0f;
	float biomeIndex = 0;
	int numOfBiomes = Settings.Biomes.Num();

	for(int i = 0; i < numOfBiomes; ++i)
	{
		if(Settings.Biomes[i].StartHeight < heightPercent)
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

float UColorGenerator::BiomeIndexFromPoint(const FVector& pointOnSphere)
{
	float heightPercent = (pointOnSphere.Z + 1) / 2.0f;
	heightPercent += (NoiseFilter->Evaluate(pointOnSphere) - Settings.NoiseOffset) * Settings.NoiseStrength;
	int biomeIndex = 0;
	int numOfBiomes = Settings.Biomes.Num();

	if(numOfBiomes == 0)
	{
		return 1;
	}
	
	for (int i = 0; i < Settings.Biomes.Num(); ++i)
	{
		if (Settings.Biomes[i].StartHeight < heightPercent)
		{
			biomeIndex = (int)Settings.Biomes[i].BiomeType;
		}
		else
		{
			break;
		}
	}
	return biomeIndex;
}

