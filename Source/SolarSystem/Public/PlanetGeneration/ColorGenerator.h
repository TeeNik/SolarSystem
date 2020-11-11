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

	UMaterialInstanceDynamic* GetPlanetMaterial();
	void UpdateElevation(const TPair<float, float>& minMax);
	void UpdateColors(TArray<FLinearColor>& vertexColors, const TPair<float, float>& minMax);
	float BiomePercentFromPoint(const FVector& pointOnSphere);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FColorSettings Settings;

private:
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MaterialInstance;
};
