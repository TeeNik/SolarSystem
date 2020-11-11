#pragma once

#include "ColorSettings.generated.h"

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FBiome
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UTexture2D* GradientTexture;
	UPROPERTY(EditAnywhere)
	FLinearColor Tint;
	UPROPERTY(EditAnywhere, meta = (ClapmMin = "0.0", ClampMax = "1.0"))
	float StartHeight;
	UPROPERTY(EditAnywhere, meta = (ClapmMin = "0.0", ClampMax = "1.0"))
	float TintPercent;
};

USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FBiomeColorSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FBiome> Biomes;
};


USTRUCT(BlueprintType)
struct SOLARSYSTEM_API FColorSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* PlanetMaterial;
	UPROPERTY(EditAnywhere)
	FBiomeColorSettings BiomeColorSettings;

};
