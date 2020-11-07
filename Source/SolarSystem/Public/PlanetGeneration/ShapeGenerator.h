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

	FVector CalculatePointOnSphere(FVector pointOnUnitSphere);

protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditAnywhere)
	FShapeSettings Settings;

private:
	UPROPERTY(Transient)
	TArray<FNoiseFilter> NoiseFilters;

	bool IsInited = false;
	void Init();
};
