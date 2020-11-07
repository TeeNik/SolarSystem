#pragma once

#include "PlanetGeneration/NoiseSettings.h"
#include "ShapeSettings.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseLayer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool Enabled = true;
	UPROPERTY(EditAnywhere)
	bool UseFirstLayerAsMask = false;
	UPROPERTY(EditAnywhere)
	FNoiseSettings NoiseSettings;

};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FShapeSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float PlanetRadius = 1.0f;
	UPROPERTY(EditAnywhere)
	TArray<FNoiseLayer> NoiseLayers;
};