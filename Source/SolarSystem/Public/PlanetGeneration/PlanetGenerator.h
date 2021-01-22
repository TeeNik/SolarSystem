#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PlanetGenerator.generated.h"

class UShapeGenerator;
class UColorGenerator;
class UPlanetMeshData;
class UMaterialInterface;

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
	UProceduralMeshComponent* CloudMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShapeGenerator* ShapeGenerator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UColorGenerator* ColorGenerator;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UShapeGenerator* CloudGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Clouds;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* CloudMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Resolution = 32;
	UPROPERTY(EditAnywhere)
	bool ShouldRegenerate = false;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

private:
	int64 GetUnixTime();
	void GenerateCubeMesh();
};