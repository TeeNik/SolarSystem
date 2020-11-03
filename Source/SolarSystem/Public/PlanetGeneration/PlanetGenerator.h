#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PlanetGenerator.generated.h"

class UShapeGenerator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLARSYSTEM_API APlanetGenerator : public AActor
{
	GENERATED_BODY()

public:	
	APlanetGenerator();
	void GenerateCubeMesh();

protected:
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UProceduralMeshComponent*> Meshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* CustomMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShapeGenerator* ShapeGenerator;

	void AddTriangle(int32 V1, int32 V2, int32 V3);

	UPROPERTY(EditAnywhere)
	int Resolution = 4;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor Color;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* Material;

	class UNoiseGenerator* Noise;

private:
	int NumOfGenerations;
	TArray<FVector>  Vertices;
	TArray<int32>  Triangles;

};
