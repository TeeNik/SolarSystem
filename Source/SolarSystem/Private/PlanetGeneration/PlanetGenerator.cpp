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

	TArray<FLinearColor> VertexColors;
	 
	FVector directions[] = { FVector::UpVector, FVector::LeftVector, FVector::DownVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };
	const int NumOfDirections = 6;

	Vertices.Init(FVector::ZeroVector, Resolution * Resolution * NumOfDirections);
	Triangles.Init(0, (Resolution - 1) * (Resolution - 1) * 6 * NumOfDirections);
	VertexColors.Init(Color, Resolution * Resolution * NumOfDirections);

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

				Vertices[i] = ShapeGenerator->CalculatePointOnSphere(pointOnUnitSphere);

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					Triangles[triIndex] = i;
					Triangles[triIndex + 1] = i + Resolution;
					Triangles[triIndex + 2] = i + Resolution + 1;
					
					Triangles[triIndex + 3] = i;
					Triangles[triIndex + 4] = i + Resolution + 1;
					Triangles[triIndex + 5] = i + 1;
					triIndex += 6;
				}
				++i;
			}
		}
	}

	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	normals.Init(FVector::ZeroVector, Vertices.Num());
	for (int x = 0; x < Triangles.Num(); x += 3)
	{
		FVector v1 = Vertices[Triangles[x + 1]] - Vertices[Triangles[x]];
		FVector v2 = Vertices[Triangles[x + 2]] - Vertices[Triangles[x]];
		FVector faceNormal = FVector::CrossProduct(v2, v1);
		faceNormal.Normalize();
	
		normals[Triangles[x]] += faceNormal;
		normals[Triangles[x + 1]] += faceNormal;
		normals[Triangles[x + 2]] += faceNormal;
	}
	
	for (int x = 0; x < normals.Num(); ++x)
	{
		normals[x].Normalize();
	}
	

	//UE_LOG(LogTemp, Log, TEXT("Calculating time: %d"), GetUnixTime() - startTime);
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, TArray<FVector2D>(), normals, tangents);
	//UE_LOG(LogTemp, Log, TEXT("CalculateTangentsForMesh: %d"), GetUnixTime() - startTime);

	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, TArray<FVector2D>(), VertexColors, tangents, true);
	Mesh->SetMaterial(0, Material);

	UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
}