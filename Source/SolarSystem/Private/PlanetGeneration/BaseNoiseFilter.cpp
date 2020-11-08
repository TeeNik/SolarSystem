#include "PlanetGeneration/BaseNoiseFilter.h"
#include "PlanetGeneration/NoiseGenerator.h"

UBaseNoiseFilter::UBaseNoiseFilter()
{
	Noise = NewObject<UNoiseGenerator>();
}