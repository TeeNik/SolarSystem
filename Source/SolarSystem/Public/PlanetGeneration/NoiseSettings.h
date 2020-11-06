#pragma once

#include "NoiseSettings.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Strength;
	UPROPERTY(EditAnywhere)
	int NumLayers;
	UPROPERTY(EditAnywhere)
	float BaseRoughness;
	UPROPERTY(EditAnywhere)
	float Roughness;
	UPROPERTY(EditAnywhere)
	float Persistence;
	UPROPERTY(EditAnywhere)
	FVector Centre;
	UPROPERTY(EditAnywhere)
	float MinValue;
	
};
