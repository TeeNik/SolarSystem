#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlanetGeneration/NoiseFilter.h"
#include "PlanetGeneration/ShapeSettings.h"
#include "ShapeGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLARSYSTEM_API UShapeGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShapeGenerator();
	
	void Init();
	float CalculateScaledElevation(float unscaledElevation);
	float CalculateUnscaledElevation(FVector pointOnUnitSphere);

	TPair<float, float> MinMax;

protected:
	UPROPERTY(EditAnywhere)
	FShapeSettings Settings;

private:
	UPROPERTY(Transient)
	TArray<class UBaseNoiseFilter*> NoiseFilters;

	UPROPERTY(Transient)
	class UNoiseFilterFactory* NoiseFilterFactory;

	void AddMinMax(float value);
	void ResetMinMax();
};