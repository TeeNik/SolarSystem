#include "PlanetGeneration/BaseNoiseFilter.h"
#include "PlanetGeneration/NoiseGenerator.h"

FBaseNoiseFilter::FBaseNoiseFilter()
{
	Noise = NewObject<UNoiseGenerator>();
}