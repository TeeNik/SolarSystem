#pragma once

#include "NoiseSettings.generated.h"

UENUM(Blueprintable)
enum class NoiseType : uint8
{
	Simple,
	Ridged,
};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	NoiseType Type;
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
