#include "PlanetGeneration/PlanetGenerator.h"
#include "PlanetGeneration/ColorGenerator.h"
#include "PlanetGeneration/ShapeGenerator.h"
#include "PlanetGeneration/NoiseGenerator.h"
#include "KismetProceduralMeshLibrary.h"

#include "ImageUtils.h"
#include "Kismet/KismetMathLibrary.h"

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


	TArray<FVector> spherePoints;
	spherePoints.Init(FVector::ZeroVector, vertices.Num());


	UE_LOG(LogTemp, Log, TEXT("Time spend for colors: %d"), GetUnixTime() - startTime);


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

				vertices[index] = pointOnUnitSphere;

				float unscaledElevation = ShapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
				float biomePercent = ColorGenerator->BiomePercentFromPoint(pointOnUnitSphere);
				//vertices[index] = pointOnUnitSphere * ShapeGenerator->CalculateScaledElevation(unscaledElevation);
				vertices[index] = pointOnUnitSphere;

				spherePoints[index] = pointOnUnitSphere;


				int biomeIndex = unscaledElevation > 0 ? ColorGenerator->BiomeIndexFromPoint(pointOnUnitSphere) : 0;
				uv[index] = FVector2D(unscaledElevation, biomeIndex);

				if(Clouds.Num() > 0 && Clouds.Num() % 2 == 0)
				{
					float clouds = 0;
					for (int i = 0; i < Clouds.Num(); i += 2)
					{
						//clouds += Clouds[i] * FMath::PerlinNoise3D(Clouds[i + 1] * pointOnUnitSphere);
						clouds += unscaledElevation;
					}
					uv[index].X = clouds;
				}

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


	TArray<FLinearColor> colors;
	colors.Init(FColor::White, vertices.Num());

	//for (int i = 0; i < colors.Num(); ++i)
	//{
	//	float biomIndex = uv[i].Y;
	//	float min = biomIndex > 0 ? 0 : ShapeGenerator->MinMax.Key;
	//	float max = biomIndex > 0 ? ShapeGenerator->MinMax.Value : 0;
	//
	//	//colors[i] = ColorGenerator->GetColorFromPoint(spherePoints[i],  uv[i].X, min, max);
	//	colors[i] = ColorGenerator->GetColor(biomIndex,  uv[i].X, min, max);
	//}

	/*
	int craterIndex = 500;
	FVector center = vertices[craterIndex];
	TArray<float> heights;
	heights.Init(0.0f, vertices.Num());

	for (int i = 0; i < vertices.Num(); ++i)
	{
		if(FVector::Dist(vertices[i], center) <= CraterRadius)
		{
			float dist = FVector::Dist(vertices[i], center);
			float x = dist / CraterRadius;
			UE_LOG(LogTemp, Log, TEXT("%f %f"), dist, x);
			heights[i] = x;

		}
		//{
		//	float x = (vertices[i] - center).Size() / CraterRadius;
		//	float cavity = x * x - 1;
		//	float rimX = FMath::Min(x - 1 - RimWidth, 0.0f);
		//	float rim = RimSteepness * rimX * rimX;
		//
		//	float craterShape = SmoothMax(cavity, FloorHeight, Smoothness);
		//	vertices[i] *= SmoothMin(craterShape, rim, Smoothness);
		//}
	}


	for(int i = 0; i < vertices.Num(); ++i)
	{
		vertices[i] *= heights[i];
	}
	*/



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

	UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	dynamicMaterial->SetVectorParameterValue(TEXT("ElevationMinMax"), FVector(ShapeGenerator->MinMax.Key, ShapeGenerator->MinMax.Value, 0));

	Mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uv, colors, TArray<FProcMeshTangent>(), true);
	Mesh->SetMaterial(0, dynamicMaterial);

	UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
}