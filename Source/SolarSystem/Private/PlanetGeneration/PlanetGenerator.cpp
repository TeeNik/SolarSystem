#include "PlanetGeneration/PlanetGenerator.h"
#include "PlanetGeneration/ColorGenerator.h"
#include "PlanetGeneration/ShapeGenerator.h"
#include "PlanetGeneration/NoiseGenerator.h"
#include "KismetProceduralMeshLibrary.h"

#include "ImageUtils.h"

APlanetGenerator::APlanetGenerator()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	ShapeGenerator = CreateDefaultSubobject<UShapeGenerator>(TEXT("ShapeGenerator"));
	ColorGenerator = CreateDefaultSubobject<UColorGenerator>(TEXT("ColorGenerator"));
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

	ShapeGenerator->Init();
	ColorGenerator->Init();

	TArray<FVector> vertices;
	vertices.Init(FVector::ZeroVector, Resolution * Resolution * NumOfDirections);
	TArray<int32> triangles;
	triangles.Init(0, (Resolution - 1) * (Resolution - 1) * 6 * NumOfDirections);
	TArray<FVector2D> uv;
	uv.Init(FVector2D::ZeroVector, vertices.Num());

	int index = 0;
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

				float unscaledElevation = ShapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
				float biomePercent = ColorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
				vertices[index] = pointOnUnitSphere * ShapeGenerator->CalculateScaledElevation(unscaledElevation);

				int biomeIndex = unscaledElevation > 0 ? ColorGenerator->BiomeIndexFromPoint(pointOnUnitSphere) : 0;
				uv[index] = FVector2D(unscaledElevation, biomeIndex);

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					triangles[triIndex] = index;
					triangles[triIndex + 1] = index + Resolution;
					triangles[triIndex + 2] = index + Resolution + 1;
					
					triangles[triIndex + 3] = index;
					triangles[triIndex + 4] = index + Resolution + 1;
					triangles[triIndex + 5] = index + 1;
					triIndex += 6;
				}
				++index;
			}
		}
	}

	TArray<FVector> normals;
	normals.Init(FVector::ZeroVector, vertices.Num());
	for (int i = 0; i < triangles.Num(); i += 3)
	{
		FVector v1 = vertices[triangles[i + 1]] - vertices[triangles[i]];
		FVector v2 = vertices[triangles[i + 2]] - vertices[triangles[i]];
		FVector faceNormal = FVector::CrossProduct(v2, v1);
		faceNormal.Normalize();
	
		normals[triangles[i]] += faceNormal;
		normals[triangles[i + 1]] += faceNormal;
		normals[triangles[i + 2]] += faceNormal;
	}
	
	for (int i = 0; i < normals.Num(); ++i)
	{
		normals[i].Normalize();
	}

	//UE_LOG(LogTemp, Log, TEXT("Calculating time: %d"), GetUnixTime() - startTime);
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, TArray<FVector2D>(), normals, tangents);
	//UE_LOG(LogTemp, Log, TEXT("CalculateTangentsForMesh: %d"), GetUnixTime() - startTime);

	UE_LOG(LogTemp, Log, TEXT("Time spend on calculation: %d"), GetUnixTime() - startTime);

	UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	dynamicMaterial->SetVectorParameterValue(TEXT("ElevationMinMax"), FVector(ShapeGenerator->MinMax.Key, ShapeGenerator->MinMax.Value, 0));

	Mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uv, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	Mesh->SetMaterial(0, dynamicMaterial);

	UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
}