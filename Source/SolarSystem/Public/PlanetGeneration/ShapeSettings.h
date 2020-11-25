#pragma once

#include "PlanetGeneration/NoiseSettings.h"
#include "ShapeSettings.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseLayer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Enabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseFirstLayerAsMask = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseSettings NoiseSettings;

};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FShapeSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlanetRadius = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNoiseLayer> NoiseLayers;
};