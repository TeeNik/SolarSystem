#include "PlanetGeneration/ColorGenerator.h"
#include "PlanetGeneration/NoiseFilterFactory.h"
#include "PlanetGeneration/BaseNoiseFilter.h"
#include "Kismet/KismetMathLibrary.h"

UColorGenerator::UColorGenerator()
{
	NoiseFilterFactory = NewObject<UNoiseFilterFactory>();
}

void UColorGenerator::Init()
{
	BiomColors.Init(FColor::White, BiomTextures.Num() * TextureSize);
	for (int i = 0; i < BiomTextures.Num(); ++i)
	{
		UTexture2D* Texture = BiomTextures[i];

		TextureCompressionSettings OldCompressionSettings = Texture->CompressionSettings;
		TextureMipGenSettings OldMipGenSettings = Texture->MipGenSettings;
		bool OldSRGB = Texture->SRGB;

		Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		Texture->SRGB = false;
		Texture->UpdateResource();

		const FColor* FormatedImageData = static_cast<const FColor*>(Texture->PlatformData->Mips[0].BulkData.LockReadOnly());

		for (float j = 0; j < 1; j += 0.01f)
		{
			BiomColors[i * TextureSize + j * TextureSize] = FormatedImageData[static_cast<int>(Texture->GetSizeX() * j)];
		}

		Texture->PlatformData->Mips[0].BulkData.Unlock();

		Texture->CompressionSettings = OldCompressionSettings;
		Texture->MipGenSettings = OldMipGenSettings;
		Texture->SRGB = OldSRGB;
		Texture->UpdateResource();
	}
	UE_LOG(LogTemp, Log, TEXT("UColorGenerator"));


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
	int biomeIndex = 1;
	int numOfBiomes = Settings.Biomes.Num();
	float blendRange = Settings.BlendAmount / 2.0f + .001f;

	if(numOfBiomes == 0)
	{
		return 1;
	}
	
	for (int i = 0; i < Settings.Biomes.Num(); ++i)
	{

		//float dist = heightPercent - Settings.Biomes[i].StartHeight;
		//float weight = UKismetMathLibrary::NormalizeToRange(dist, -blendRange, blendRange);
		//biomeIndex *= 1 - weight;
		//biomeIndex += i * weight;

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

FColor UColorGenerator::GetColorFromPoint(float biomeIndex, float value, float min, float max)
{
	//int biomIndex = BiomeIndexFromPoint(pointOnSphere);
	float normilizedValue = UKismetMathLibrary::NormalizeToRange(value, min, max);

	int floorBiome = FMath::Floor(biomeIndex);
	int ceilBiome = biomeIndex > 0.5f ? floorBiome + 1 : floorBiome - 1;

	FLinearColor floorColor = BiomColors[TextureSize * floorBiome + normilizedValue * TextureSize];
	FLinearColor ceilColor = BiomColors[TextureSize * ceilBiome + normilizedValue * TextureSize];
	FLinearColor color = (floorColor * (biomeIndex - floorBiome)) * (ceilColor * (ceilBiome - biomeIndex));
	return color.ToFColor(false);
}