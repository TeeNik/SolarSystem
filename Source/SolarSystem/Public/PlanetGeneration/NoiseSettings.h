#pragma once

#include "NoiseSettings.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Strength = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int NumLayers = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BaseRoughness = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Roughness = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Persistence = .5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Centre;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MinValue;
	
};
