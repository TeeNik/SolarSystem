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

	float CalculateScaledElevation(float unscaledElevation);
	float CalculateUnscaledElevation(FVector pointOnUnitSphere);

	TPair<float, float> MinMax;

protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere)
	FShapeSettings Settings;

private:
	UPROPERTY(Transient)
	TArray<class UBaseNoiseFilter*> NoiseFilters;

	UPROPERTY(Transient)
	class UNoiseFilterFactory* NoiseFilterFactory;

	bool IsInited = false;
	void Init();
	void AddMinMax(float value);
};
