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

	UPROPERTY(EditAnywhere)
	int Resolution = 32;
	UPROPERTY(EditAnywhere)
	bool ShouldRegenerate = false;
	UPROPERTY(EditAnywhere)
	FLinearColor Color;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* Material;

private:
	int64 GetUnixTime();
	void GenerateCubeMesh();
};
