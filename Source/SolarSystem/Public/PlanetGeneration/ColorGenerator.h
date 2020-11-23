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

protected:
	UPROPERTY(EditAnywhere)
	FColorSettings Settings;
	UPROPERTY(Transient)
	class UNoiseFilterFactory* NoiseFilterFactory;
	UPROPERTY(Transient)
	class UBaseNoiseFilter* NoiseFilter;
};
