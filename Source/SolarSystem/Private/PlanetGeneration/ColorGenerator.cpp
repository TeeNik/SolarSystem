#include "PlanetGeneration/ColorGenerator.h"
#include "PlanetGeneration/NoiseFilterFactory.h"
#include "PlanetGeneration/BaseNoiseFilter.h"
#include "Kismet/KismetMathLibrary.h"
#include <iterator>

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

			//switch (i)
			//{
			//case 1:
			//	BiomColors[i * TextureSize + j * TextureSize] = FColor::Red;
			//	continue;
			//case 2:
			//	BiomColors[i * TextureSize + j * TextureSize] = FColor::Black;
			//	continue;
			//case 3:
			//	BiomColors[i * TextureSize + j * TextureSize] = FColor::Yellow;
			//	continue;
			//}

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
	int biomeIndex = 0;
	int numOfBiomes = Settings.Biomes.Num();

	if (numOfBiomes == 0)
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

/*
FColor UColorGenerator::GetColorFromPoint(FVector pointOnSphere, float value, float min, float max)
{
	float heightPercent = (pointOnSphere.Z + 1) / 2.0f;
	heightPercent += (NoiseFilter->Evaluate(pointOnSphere) - Settings.NoiseOffset) * Settings.NoiseStrength;
	int numOfBiomes = Settings.Biomes.Num();
	float blendRange = Settings.BlendAmount / 2.0f + .001f;

	//UE_LOG(LogTemp, Log, TEXT("value min max %f %f %f"), value, min, max);

	heightPercent = FMath::Clamp(heightPercent, 0.0f, 1.0f);

	if (numOfBiomes == 0)
	{
		return FColor::White;
	}

	if(value < 0)
	{
		//water
		return GetColor(0, value, min, max);
	}

	//FLinearColor result;
	for (int i = 0; i < numOfBiomes; ++i)
	{
		if (i == numOfBiomes - 1 && Settings.Biomes[i].StartHeight < heightPercent || i < numOfBiomes - 1 && Settings.Biomes[i+1].StartHeight > heightPercent && Settings.Biomes[i].StartHeight <= heightPercent)
		{
			if (i < numOfBiomes - 1 && heightPercent > Settings.Biomes[i + 1].StartHeight - blendRange)
			{
				//blend current and next	
				FLinearColor current = GetColor((float)Settings.Biomes[i].BiomeType, value, min, max);
				FLinearColor next = GetColor((float)Settings.Biomes[i+1].BiomeType, value, min, max);
				float blendValue = UKismetMathLibrary::NormalizeToRange(heightPercent, Settings.Biomes[i + 1].StartHeight - blendRange, Settings.Biomes[i + 1].StartHeight + blendRange);
				UE_LOG(LogTemp, Log, TEXT("%f"), blendValue);
				FLinearColor result = current * (1-blendValue) + next * blendValue;
				//return FColor::Red;
				return result.ToFColor(true);
			}
			//if (i > 0 && heightPercent < Settings.Biomes[i].StartHeight + blendRange)
			//{
			//	//blend current and prev
			//	FLinearColor current = GetColor((float)Settings.Biomes[i].BiomeType, value, min, max);
			//	FLinearColor prev = GetColor((float)Settings.Biomes[i - 1].BiomeType, value, min, max);
			//	float blendValue = UKismetMathLibrary::NormalizeToRange(heightPercent, Settings.Biomes[i].StartHeight - blendRange, Settings.Biomes[i].StartHeight + blendRange);
			//	FLinearColor result = current * (1 - blendValue) + prev * blendValue;
			//	//return FColor::Blue;
			//	return result.ToFColor(true);
			//}
			//return current
			return GetColor((float)Settings.Biomes[i].BiomeType, value, min, max);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("heightPercent %f"), heightPercent);
	return FColor::White;
}
*/

FColor UColorGenerator::GetColor(float biomeIndex, float value, float min, float max)
{
	float normilizedValue = UKismetMathLibrary::NormalizeToRange(value, min, max);
	if(TextureSize * biomeIndex + normilizedValue * (TextureSize-1) >= BiomColors.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("%f %f %f %f"), biomeIndex, value, normilizedValue);
	}
	return BiomColors[TextureSize * biomeIndex + normilizedValue * (TextureSize-1)];
}
