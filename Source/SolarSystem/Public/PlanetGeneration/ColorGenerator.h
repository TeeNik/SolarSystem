#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlanetGeneration/ColorSettings.h"
#include "ColorGenerator.generated.h"

class UMaterialInstanceDynamic;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLARSYSTEM_API UColorGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UColorGenerator();

	void Init();
	float BiomePercentFromPoint(const FVector& pointOnSphere);
	float BiomeIndexFromPoint(const FVector& pointOnSphere);

	FColor GetColor(float biomeIndex, float value, float min, float max);
	FColor GetColorFromPoint(FVector pointOnSphere, float value, float min, float max);

protected:
	UPROPERTY(EditAnywhere)
	FColorSettings Settings;
	UPROPERTY(Transient)
	class UNoiseFilterFactory* NoiseFilterFactory;
	UPROPERTY(Transient)
	class UBaseNoiseFilter* NoiseFilter;

	UPROPERTY(EditAnywhere)
	TArray<class UTexture2D*> BiomTextures;

	UPROPERTY(EditAnywhere)
	bool DebugBlendingRange;
	UPROPERTY(EditAnywhere)
	bool DebugBlending;

private:
	TArray<FColor> BiomColors;
	int TextureSize = 100;
};
