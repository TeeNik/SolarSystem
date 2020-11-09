#include "PlanetGeneration/PlanetGenerator.h"
#include "PlanetGeneration/ShapeGenerator.h"
#include "PlanetGeneration/NoiseGenerator.h"
#include "KismetProceduralMeshLibrary.h"

APlanetGenerator::APlanetGenerator()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	ShapeGenerator = CreateDefaultSubobject<UShapeGenerator>(TEXT("ShapeGenerator"));
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->bUseAsyncCooking = true;
}

void APlanetGenerator::CallMeshGeneration()
{
	if (ShouldRegenerate)
	{
		GenerateCubeMesh();
	}
}

void APlanetGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateCubeMesh();
}

void APlanetGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CallMeshGeneration();
}

int64 APlanetGenerator::GetUnixTime()
{
	FDateTime timeUtc = FDateTime::UtcNow();
	return timeUtc.ToUnixTimestamp() * 1000 + timeUtc.GetMillisecond();
}

void APlanetGenerator::GenerateCubeMesh()
{
	int64 startTime = GetUnixTime();

	FVector directions[] = { FVector::UpVector, FVector::LeftVector, FVector::DownVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };
	const int NumOfDirections = 6;

	TArray<FVector> vertices;
	vertices.Init(FVector::ZeroVector, Resolution * Resolution * NumOfDirections);
	TArray<int32> triangles;
	triangles.Init(0, (Resolution - 1) * (Resolution - 1) * 6 * NumOfDirections);
	TArray<FLinearColor> vertexColors;
	vertexColors.Init(Color, Resolution * Resolution * NumOfDirections);

	int i = 0;
	int triIndex = 0;

	for (int j = 0; j < NumOfDirections; ++j)
	{
		FVector localUp = directions[j];
		FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
		FVector axisB = FVector::CrossProduct(localUp, axisA);

		for (int y = 0; y < Resolution; ++y)
		{
			for (int x = 0; x < Resolution; ++x)
			{
				FVector2D percent = FVector2D(x, y) / (Resolution - 1);
				FVector pointOnCube = localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
				FVector pointOnUnitSphere = pointOnCube;
				pointOnUnitSphere.Normalize();

				float elevation = ShapeGenerator->CalculateElevation(pointOnUnitSphere);
				vertices[i] = pointOnUnitSphere * elevation;
				vertexColors[i] = FVector(elevation, 0, 0);

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					triangles[triIndex] = i;
					triangles[triIndex + 1] = i + Resolution;
					triangles[triIndex + 2] = i + Resolution + 1;
					
					triangles[triIndex + 3] = i;
					triangles[triIndex + 4] = i + Resolution + 1;
					triangles[triIndex + 5] = i + 1;
					triIndex += 6;
				}
				++i;
			}
		}
	}

	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	normals.Init(FVector::ZeroVector, vertices.Num());
	for (int x = 0; x < triangles.Num(); x += 3)
	{
		FVector v1 = vertices[triangles[x + 1]] - vertices[triangles[x]];
		FVector v2 = vertices[triangles[x + 2]] - vertices[triangles[x]];
		FVector faceNormal = FVector::CrossProduct(v2, v1);
		faceNormal.Normalize();
	
		normals[triangles[x]] += faceNormal;
		normals[triangles[x + 1]] += faceNormal;
		normals[triangles[x + 2]] += faceNormal;
	}
	
	for (int x = 0; x < normals.Num(); ++x)
	{
		normals[x].Normalize();
	}

	for (int x = 0; x < vertexColors.Num(); ++x)
	{
		float value = (vertexColors[x].R - ShapeGenerator->MinMax.Key) / (ShapeGenerator->MinMax.Value - ShapeGenerator->MinMax.Key);
		vertexColors[x] = FVector(FMath::Max(value, 0.01f), 0, 0);;
	}

	//UE_LOG(LogTemp, Log, TEXT("Calculating time: %d"), GetUnixTime() - startTime);
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, TArray<FVector2D>(), normals, tangents);
	//UE_LOG(LogTemp, Log, TEXT("CalculateTangentsForMesh: %d"), GetUnixTime() - startTime);

	UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	dynamicMaterial->SetVectorParameterValue(TEXT("ElevationMinMax"), FVector(ShapeGenerator->MinMax.Key, ShapeGenerator->MinMax.Value, 0));

	Mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, TArray<FVector2D>(), vertexColors, tangents, true);
	Mesh->SetMaterial(0, dynamicMaterial);

	UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
}