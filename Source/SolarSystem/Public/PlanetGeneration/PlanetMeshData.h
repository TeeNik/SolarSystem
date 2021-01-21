#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlanetMeshData.generated.h"

UCLASS()
class SOLARSYSTEM_API UPlanetMeshData : public UObject
{
	GENERATED_BODY()

public:
	void GenerateData(int resolution);
	void RecalculateNormals();

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	int Resolution;

};
