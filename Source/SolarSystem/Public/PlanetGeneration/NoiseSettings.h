#pragma once

#include "NoiseSettings.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Strength = 1;
	UPROPERTY(EditAnywhere)
	int NumLayers = 1;
	UPROPERTY(EditAnywhere)
	float BaseRoughness = 1;
	UPROPERTY(EditAnywhere)
	float Roughness = 2;
	UPROPERTY(EditAnywhere)
	float Persistence = .5f;
	UPROPERTY(EditAnywhere)
	FVector Centre;
	UPROPERTY(EditAnywhere)
	float MinValue;
	
};
