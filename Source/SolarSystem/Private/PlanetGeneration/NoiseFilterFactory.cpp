#include "PlanetGeneration/NoiseFilterFactory.h"
#include "PlanetGeneration/BaseNoiseFilter.h"
#include "PlanetGeneration/NoiseFilter.h"
#include "PlanetGeneration/RidgedNoiseFilter.h"
#include "PlanetGeneration/NoiseSettings.h"

UBaseNoiseFilter* UNoiseFilterFactory::GetNoiseFilter(const FNoiseSettings& settings)
{
	UBaseNoiseFilter* noiseFilter;
	switch (settings.Type)
	{
	case NoiseType::Simple:
		noiseFilter = NewObject<UNoiseFilter>();
		break;
	case NoiseType::Ridged:
		noiseFilter = NewObject<URidgedNoiseFilter>();
		break;
	default:
		return nullptr;
	}
	noiseFilter->SetSettings(settings);
	return noiseFilter;
}
