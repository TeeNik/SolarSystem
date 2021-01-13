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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	BiomeType BiomeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClapmMin = "0.0", ClampMax = "1.0"))
	float StartHeight;
};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FColorSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBiome> Biomes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseSettings NoiseSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendAmount;
};