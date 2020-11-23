#pragma once
#include "PlanetGeneration/NoiseSettings.h"
#include "ColorSettings.generated.h"

UENUM(Blueprintable)
enum class BiomeType : uint8
{
	Ocean,
	Mountains,
	Sand,
	Tundra,
};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FBiome
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	BiomeType BiomeType;
	UPROPERTY(EditAnywhere, meta = (ClapmMin = "0.0", ClampMax = "1.0"))
	float StartHeight;
};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FColorSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FBiome> Biomes;
	UPROPERTY(EditAnywhere)
	FNoiseSettings NoiseSettings;
	UPROPERTY(EditAnywhere)
	float NoiseOffset;
	UPROPERTY(EditAnywhere)
	float NoiseStrength;
};