#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PlanetGenerator.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLARSYSTEM_API APlanetGenerator : public AActor
{
	GENERATED_BODY()

public:	
	APlanetGenerator();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UProceduralMeshComponent*> Meshes; //CustomMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;


	TArray<FVector>  Vertices;
	TArray<int32>  Triangles;

	void AddTriangle(int32 V1, int32 V2, int32 V3);
	void GenerateCubeMesh();

	UPROPERTY(EditDefaultsOnly)
	int Resolution = 4;

	//FVector localUp;
	//FVector axisA;
	//FVector axisB;

};
