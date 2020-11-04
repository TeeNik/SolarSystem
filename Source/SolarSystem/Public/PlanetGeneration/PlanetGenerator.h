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

	UPROPERTY(EditAnywhere)
	int Resolution = 32;
	UPROPERTY(EditAnywhere)
	bool ShouldRegenerate = false;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor Color;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* Material;

private:
	int NumOfGenerations;
	TArray<FVector>  Vertices;
	TArray<int32>  Triangles;

	int64 GetUnixTime();

	void GenerateCubeMesh();
};
