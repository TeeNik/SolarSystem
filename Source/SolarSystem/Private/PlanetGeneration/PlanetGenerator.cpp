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

	//for(int i = 0; i < 6; ++i)
	//{
	//	Meshes.Add(CreateDefaultSubobject<UProceduralMeshComponent>(names[i]));
	//	Meshes[i]->bUseAsyncCooking = true;
	//	Meshes[i]->SetupAttachment(Root);
	//}

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CustomMesh"));

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
	 
	FVector directions[] = { FVector::UpVector, FVector::LeftVector, FVector::DownVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };
	const int NumOfDirections = 6;

	Vertices.Empty();
	Triangles.Empty();

	for (int j = 0; j < NumOfDirections; ++j)
	{
		FVector localUp = directions[j];
		FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
		FVector axisB = FVector::CrossProduct(localUp, axisA);

		float triangleOffset = Vertices.Num();

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
				Vertices.Add(ShapeGenerator->CalculatePointOnSphere(pointOnUnitSphere));

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					Triangles.Add(i + triangleOffset);
					Triangles.Add(i + Resolution + triangleOffset);
					Triangles.Add(i + Resolution + 1 + triangleOffset);

					Triangles.Add(i + triangleOffset);
					Triangles.Add(i + Resolution + 1 + triangleOffset);
					Triangles.Add(i + 1 + triangleOffset);
					triIndex += 6;
				}
				++i;
			}
		}
	}

	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, TArray<FVector2D>(), normals, tangents);
	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, normals, TArray<FVector2D>(), VertexColors, tangents, true);
	CustomMesh->SetMaterial(0, Material);

	++NumOfGenerations;
	UE_LOG(LogTemp, Log, TEXT("NumOfGenerations: %d"), NumOfGenerations);
}