#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PlanetGenerator.generated.h"

class UShapeGenerator;
class UColorGenerator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLARSYSTEM_API APlanetGenerator : public AActor
{
	GENERATED_BODY()

public:	
	APlanetGenerator();

	UFUNCTION(BlueprintCallable)
	void CallMeshGeneration();

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShapeGenerator* ShapeGenerator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UColorGenerator* ColorGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Resolution = 32;
	UPROPERTY(EditAnywhere)
	bool ShouldRegenerate = false;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* Material;
	UPROPERTY(EditAnywhere)
	class UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RimWidth = 1.23f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RimSteepness = .42f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloorHeight = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CraterRadius = .7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Smoothness = .5f;

private:
	int64 GetUnixTime();
	void GenerateCubeMesh();

	float CavityShape(float x)
	{
		return x * x;
		//return x * x - 1;
	}
	float RimShape(float x)
	{
		x = FMath::Abs(x) - 1 - RimWidth;
		return RimSteepness * x * x;
	}
	float FloorShape(float x)
	{
		return FloorHeight;
	}
	float CraterShape(float x)
	{
		return FMath::Max(FMath::Min(CavityShape(x), RimShape(x)), FloorShape(x));
	}
	//float SmoothMin(float a, float b, float k)
	//{
	//	float h = FMath::Clamp((b - a + k) / (2 * k), 0.0f, 1.0f);
	//	return a * h + b * (1 - h) - k * h * (1 - h);
	//}
	float SmoothMin(float a, float b, float k) {
		k = FMath::Max(0.0f, k);
		float h = FMath::Max(0.0f, FMath::Min(1.0f, (b - a + k) / (2.0f * k)));
		return a * h + b * (1 - h) - k * h * (1 - h);
	}
	float SmoothMax(float a, float b, float k) {
		k = FMath::Min(0.0f, -k);
		float h = FMath::Max(0.0f, FMath::Min(1.0f, (b - a + k) / (2.0f * k)));
		return a * h + b * (1 - h) - k * h * (1 - h);
	}

};
