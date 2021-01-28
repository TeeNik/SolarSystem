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

		for (int j = 0; j < TextureSize; ++j)
		{
			BiomColors[i * TextureSize + j] = FormatedImageData[static_cast<int>(Texture->GetSizeX() * (float)j / TextureSize)];
		}

		Texture->PlatformData->Mips[0].BulkData.Unlock();

		Texture->CompressionSettings = OldCompressionSettings;
		Texture->MipGenSettings = OldMipGenSettings;
		Texture->SRGB = OldSRGB;
		Texture->UpdateResource();
	}

	NoiseFilter = NoiseFilterFactory->GetNoiseFilter(Settings.NoiseSettings);
}

float UColorGenerator::BiomeIndexFromPoint(const FVector& pointOnSphere)
{
	float heightPercent = (pointOnSphere.Z + 1) / 2.0f;
	heightPercent += (NoiseFilter->Evaluate(pointOnSphere) - Settings.NoiseOffset) * Settings.NoiseStrength;
	int biomeIndex = 0;
	heightPercent = FMath::Clamp(heightPercent, 0.0f, 1.0f);
	for (int i = 0; i < Settings.Biomes.Num(); ++i)
	{
		if (heightPercent <= Settings.Biomes[i].StartHeight)
		{
			return (int)Settings.Biomes[i].BiomeType;
		}
	}
	return biomeIndex;
}

FColor UColorGenerator::GetColorFromPoint(FVector pointOnSphere, float value, float min, float max)
{
	float heightPercent = (pointOnSphere.Z + 1) / 2.0f;
	heightPercent += (NoiseFilter->Evaluate(pointOnSphere) - Settings.NoiseOffset) * Settings.NoiseStrength;
	int numOfBiomes = Settings.Biomes.Num();
	float blendRange = Settings.BlendAmount + .001f;

	heightPercent = FMath::Clamp(heightPercent, 0.0f, 1.0f);

	if(value <= 0)
	{
		return GetColor((float)BiomeType::Ocean, value, min, max);
	}

	for (int i = 0; i < numOfBiomes; ++i)
	{
		if(heightPercent <= Settings.Biomes[i].StartHeight)
		{
			if(heightPercent > Settings.Biomes[i].StartHeight - blendRange && i < numOfBiomes - 1)
			{
				FLinearColor current = GetColor((float)Settings.Biomes[i].BiomeType, value, min, max);
				FLinearColor next = GetColor((float)Settings.Biomes[i+1].BiomeType, value, min, max);
				float blendValue = UKismetMathLibrary::NormalizeToRange(heightPercent, Settings.Biomes[i].StartHeight - blendRange, Settings.Biomes[i].StartHeight);
				FLinearColor result = current * (1 - blendValue) + next * blendValue;
				return result.ToFColor(true);
			}
			return GetColor((float)Settings.Biomes[i].BiomeType, value, min, max);
		}
	}
	return FColor::Purple;
}

FColor UColorGenerator::GetColor(float biomeIndex, float value, float min, float max)
{
	float normilizedValue = UKismetMathLibrary::NormalizeToRange(value, min, max);
	return BiomColors[TextureSize * biomeIndex + normilizedValue * (TextureSize-1)];
}
