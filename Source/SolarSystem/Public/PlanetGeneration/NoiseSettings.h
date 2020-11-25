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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	NoiseType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumLayers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRoughness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Roughness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Persistence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Centre;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;
	
};
