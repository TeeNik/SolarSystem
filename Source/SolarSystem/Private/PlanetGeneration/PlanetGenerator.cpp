#include "PlanetGeneration/PlanetGenerator.h"
#include "PlanetGeneration/ShapeGenerator.h"
#include "PlanetGeneration/NoiseGenerator.h"
#include "KismetProceduralMeshLibrary.h"

APlanetGenerator::APlanetGenerator()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ShapeGenerator = CreateDefaultSubobject<UShapeGenerator>(TEXT("ShapeGenerator"));
	RootComponent = Root;

	FName names[] = { TEXT("Mesh1"), TEXT("Mesh2"), TEXT("Mesh3"), TEXT("Mesh4"), TEXT("Mesh5"), TEXT("Mesh6") };

	for(int i = 0; i < 6; ++i)
	{
		Meshes.Add(CreateDefaultSubobject<UProceduralMeshComponent>(names[i]));
		Meshes[i]->bUseAsyncCooking = true;
		Meshes[i]->SetupAttachment(Root);
	}

	Noise = NewObject<UNoiseGenerator>();
	Noise->Randomize(0);
}

void APlanetGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateCubeMesh();
}

void APlanetGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateCubeMesh();
}

void APlanetGenerator::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GenerateCubeMesh();
}

void APlanetGenerator::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void APlanetGenerator::GenerateCubeMesh()
{
	TArray<FLinearColor> VertexColors;
	VertexColors.Init(Color, Resolution * Resolution);
	 
	FVector directions[] = { FVector::UpVector, FVector::DownVector, FVector::LeftVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };

	for (int j = 0; j < Meshes.Num(); ++j)
	{
		UProceduralMeshComponent* mesh = Meshes[j];
		mesh->ClearAllMeshSections();
		FVector localUp = directions[j];
		FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
		FVector axisB = FVector::CrossProduct(localUp, axisA);

		Vertices.Empty();
		Triangles.Empty();

		Vertices.Init(FVector::ZeroVector, Resolution * Resolution);
		//Triangles.Init(0, (Resolution - 1) * (Resolution - 1) * 6);

		int i = 0;
		int triIndex = 0;
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
					Triangles.Add(i);
					Triangles.Add(i + Resolution);
					Triangles.Add(i + Resolution + 1);

					Triangles.Add(i);
					Triangles.Add(i + Resolution + 1);
					Triangles.Add(i + 1);
					triIndex += 6;
				}

				++i;
			}
		}

		TArray<FVector> normals;
		TArray<FProcMeshTangent> tangents;

		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, TArray<FVector2D>(), normals, tangents);
		mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, TArray<FVector2D>(), VertexColors, tangents, true);
		mesh->SetMaterial(0, Material);
	}

	++NumOfGenerations;
	UE_LOG(LogTemp, Log, TEXT("NumOfGenerations: %d"), NumOfGenerations);
}